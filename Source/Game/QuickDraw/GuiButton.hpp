#pragma once

#include <functional>
#include <string>

class GuiButton
{
public:
    using ClickCallback = std::function<void()>;

    GuiButton(int x, int y, int width, int height, std::string text);
    ~GuiButton() = default;

    GuiButton(const GuiButton&) = delete;
    GuiButton& operator=(const GuiButton&) = delete;


    void Draw();

    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void SetText(std::string text);

    void SetFocus(bool focused);
    bool IsFocused() const;

    void SetActive(bool active);
    bool IsActive() const;

    void SetDescription(std::string description);
    const std::string& GetDescription() const;

    void SetImage(void* textureId);
    void SetImageTint(unsigned int rgba);
    void ClearImage();

    void SetColors(unsigned int normal, unsigned int hovered,
                   unsigned int active, unsigned int text);
    void SetTextColor(unsigned int rgba);

    ClickCallback onClick;

    int x;
    int y;
    int width;
    int height;
    std::string label;

private:
    bool IsMouseOver() const;

    bool active_ = true;
    bool focused_ = false;
    std::string description_;
    void* textureId_ = nullptr;

    unsigned int normalColor_  = 0x181818FF;
    unsigned int hoveredColor_ = 0x585858FF;
    unsigned int activeColor_  = 0xFF969696;
    unsigned int textColor_    = 0xFFFFFFFF;
    unsigned int imageTint_    = 0xFFFFFFFF;
};
