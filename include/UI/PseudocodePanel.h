#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace UI {

class PseudocodePanel {
public:
    PseudocodePanel();

    /// Replace the current pseudocode content.
    void setPseudocode(const std::string& title, const std::vector<std::string>& lines);

    /// Set which line is highlighted (0-indexed, -1 = none).
    void setHighlightedLine(int lineIndex);
    int  getHighlightedLine() const;

    void show();
    void hide();
    bool isVisible() const;

    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

private:
    std::string title;
    std::vector<std::string> lines;
    int highlightedLine = -1;
    bool visible = false;

    sf::Font* font = nullptr;

    // Layout constants
    static constexpr float kPanelWidth   = 280.0f;
    static constexpr float kPanelPadding = 14.0f;
    static constexpr float kLineHeight   = 24.0f;
    static constexpr float kTitleHeight  = 32.0f;
    static constexpr float kCornerRadius = 8.0f;
    static constexpr float kHighlightBarWidth = 3.0f;
    static constexpr float kPanelRightMargin  = 20.0f;
    static constexpr float kPanelTopMargin    = 170.0f;

    // Colors
    static inline const sf::Color kBgColor          {15, 23, 42, 234};       // dark navy
    static inline const sf::Color kTitleColor        {148, 163, 184};         // slate-400
    static inline const sf::Color kLineColor         {226, 232, 240};         // slate-200
    static inline const sf::Color kLineNumberColor   {100, 116, 139};         // slate-500
    static inline const sf::Color kHighlightBg       {59, 130, 246, 77};      // blue-500 / 30%
    static inline const sf::Color kHighlightBar      {59, 130, 246};          // blue-500
    static inline const sf::Color kHighlightText     {255, 255, 255};         // white
};

} // namespace UI
