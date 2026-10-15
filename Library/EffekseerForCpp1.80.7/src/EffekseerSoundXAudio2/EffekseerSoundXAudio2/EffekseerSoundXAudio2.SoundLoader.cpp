
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerSoundXAudio2.SoundLoader.h"
#include "../../Effekseer/Effekseer/Utils/Effekseer.BinaryReader.h"
#include "EffekseerSoundXAudio2.SoundImplemented.h"
#include <assert.h>
#include <memory>
#include <stdint.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerSound
{

namespace SupportXAudio2
{
class BinaryFileReader : public Effekseer::FileReader
{
private:
	Effekseer::BinaryReader<true> reader_;
	size_t size_ = 0;

public:
	BinaryFileReader(const void* data, int32_t size)
		: reader_(static_cast<const uint8_t*>(data), size >= 0 ? static_cast<size_t>(size) : 0)
		, size_(size >= 0 ? static_cast<size_t>(size) : 0)
	{
	}

	virtual ~BinaryFileReader()
	{
	}

	size_t Read(void* buffer, size_t size) override
	{
		if (buffer == nullptr)
			return 0;
		size = size < reader_.GetRemainingSize() ? size : reader_.GetRemainingSize();
		if (!reader_.ReadBytes(buffer, size))
			return 0;
		return size;
	}

	void Seek(int position) override
	{
		if (position < 0 || !reader_.SetOffset(static_cast<size_t>(position)))
			return;
	}

	int GetPosition() const override
	{
		return static_cast<int>(reader_.GetOffset());
	}

	size_t GetLength() const override
	{
		return size_;
	}
};
} // namespace SupportXAudio2

SoundLoader::SoundLoader(const SoundImplementedRef& sound, ::Effekseer::FileInterfaceRef fileInterface)
	: sound_(sound)
	, fileInterface_(fileInterface)
{
	if (fileInterface_ == nullptr)
	{
		fileInterface_ = Effekseer::MakeRefPtr<Effekseer::DefaultFileInterface>();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundLoader::~SoundLoader()
{
}

::Effekseer::SoundDataRef SoundLoader::Load(::Effekseer::FileReaderRef reader)
{
	uint32_t chunkIdent, chunkSize;
	// check RIFF chunk
	if (reader->Read(&chunkIdent, 4) != 4 || reader->Read(&chunkSize, 4) != 4)
		return nullptr;
	if (memcmp(&chunkIdent, "RIFF", 4) != 0)
	{
		return nullptr;
	}

	// check WAVE symbol
	if (reader->Read(&chunkIdent, 4) != 4)
		return nullptr;
	if (memcmp(&chunkIdent, "WAVE", 4) != 0)
	{
		return nullptr;
	}

	WAVEFORMATEX wavefmt = {0};
	for (;;)
	{
		if (reader->Read(&chunkIdent, 4) != 4 || reader->Read(&chunkSize, 4) != 4)
			return nullptr;

		if (memcmp(&chunkIdent, "fmt ", 4) == 0)
		{
			// format chunk
			uint32_t size = (chunkSize < (uint32_t)sizeof(wavefmt)) ? chunkSize : (uint32_t)sizeof(wavefmt);
			if (reader->Read(&wavefmt, size) != size)
				return nullptr;
			if (size < chunkSize)
			{
				if (chunkSize - size > reader->GetLength() - static_cast<size_t>(reader->GetPosition()))
					return nullptr;
				reader->Seek(reader->GetPosition() + static_cast<int32_t>(chunkSize - size));
			}
		}
		else if (memcmp(&chunkIdent, "data", 4) == 0)
		{
			// data chunk
			break;
		}
		else
		{
			// unknown chunk
			if (chunkSize > reader->GetLength() - static_cast<size_t>(reader->GetPosition()))
				return nullptr;
			reader->Seek(reader->GetPosition() + static_cast<int32_t>(chunkSize));
		}
	}

	// check a format
	if (wavefmt.wFormatTag != WAVE_FORMAT_PCM || wavefmt.nChannels == 0 || wavefmt.nChannels > 2 || chunkSize > reader->GetLength() - static_cast<size_t>(reader->GetPosition()))
	{
		return nullptr;
	}

	uint8_t* buffer;
	uint32_t size;
	switch (wavefmt.wBitsPerSample)
	{
	case 8:
		// convert 8bit -> 16bit PCM
		if (chunkSize > UINT32_MAX / 2)
			return nullptr;
		size = chunkSize * 2;
		buffer = new uint8_t[size];
		if (reader->Read(&buffer[size / 2], chunkSize) != chunkSize)
		{
			delete[] buffer;
			return nullptr;
		}
		{
			int16_t* dst = (int16_t*)&buffer[0];
			uint8_t* src = (uint8_t*)&buffer[size / 2];
			for (uint32_t i = 0; i < size; i += 2)
			{
				*dst++ = (int16_t)(((int32_t)*src++ - 128) << 8);
			}
		}
		break;
	case 16:
		// not convert
		buffer = new uint8_t[chunkSize];
		size = static_cast<uint32_t>(reader->Read(buffer, chunkSize));
		break;
	case 24:
		// convert 24bit -> 16bit PCM
		size = chunkSize * 2 / 3;
		buffer = new uint8_t[size];
		{
			uint8_t* chunkData = new uint8_t[chunkSize];
			if (reader->Read(chunkData, chunkSize) != chunkSize)
			{
				delete[] chunkData;
				delete[] buffer;
				return nullptr;
			}

			int16_t* dst = (int16_t*)&buffer[0];
			uint8_t* src = (uint8_t*)&chunkData[0];
			for (uint32_t i = 0; i < size; i += 2)
			{
				*dst++ = (int16_t)(src[1] | (src[2] << 8));
				src += 3;
			}
			delete[] chunkData;
		}
		break;
	default:
		return nullptr;
	}

	SoundDataRef soundData = ::Effekseer::MakeRefPtr<SoundData>();
	soundData->channels_ = wavefmt.nChannels;
	soundData->sampleRate_ = wavefmt.nSamplesPerSec;
	soundData->buffer_.Flags = XAUDIO2_END_OF_STREAM;
	soundData->buffer_.AudioBytes = size;
	soundData->buffer_.pAudioData = (BYTE*)buffer;

	return soundData;
}

::Effekseer::SoundDataRef SoundLoader::Load(const char16_t* path)
{
	assert(path != nullptr);

	auto reader = fileInterface_->OpenRead(path);
	if (reader == nullptr)
		return nullptr;

	return Load(reader);
}

::Effekseer::SoundDataRef SoundLoader::Load(const void* data, int32_t size)
{
	auto reader = Effekseer::MakeRefPtr<SupportXAudio2::BinaryFileReader>(data, size);
	return Load(reader);
}

void SoundLoader::Unload(::Effekseer::SoundDataRef soundData)
{
	if (soundData != nullptr)
	{
		// stop a voice which plays this data
		sound_->StopData(soundData);
		SoundData* soundDataImpl = (SoundData*)soundData.Get();
		ES_SAFE_DELETE_ARRAY(soundDataImpl->buffer_.pAudioData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerSound
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
