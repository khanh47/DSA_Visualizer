#include "PseudocodePanel.h"
#include "RoundedRect.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cmath>

namespace UI {

PseudocodePanel::PseudocodePanel() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

void PseudocodePanel::setPseudocode(const std::string& newTitle,
                                     const std::vector<std::string>& newLines) {
    title = newTitle;
    lines = newLines;
    highlightedLine = -1;
    visible = true;
}

void PseudocodePanel::setHighlightedLine(int lineIndex) {
    highlightedLine = lineIndex;
}

int PseudocodePanel::getHighlightedLine() const {
    return highlightedLine;
}

void PseudocodePanel::show()  { visible = true; }
void PseudocodePanel::hide()  { visible = false; }
bool PseudocodePanel::isVisible() const { return visible; }

void PseudocodePanel::processEvent(const sf::Event& event) {
    (void)event;
    // Reserved for future interactivity (close button, drag, etc.)
}

void PseudocodePanel::render(sf::RenderWindow& window) {
    if (!visible || lines.empty() || !font) return;

    const sf::Vector2u windowSize = window.getSize();
    const float windowWidth  = static_cast<float>(windowSize.x);
    const float windowHeight = static_cast<float>(windowSize.y);

    // Calculate panel dimensions
    const float contentHeight = kTitleHeight + kLineHeight * static_cast<float>(lines.size())
                                + kPanelPadding * 2.0f;
    const float panelHeight = std::min(contentHeight, windowHeight - kPanelTopMargin - 80.0f);

    const float panelX = windowWidth - kPanelWidth - kPanelRightMargin;
    const float panelY = kPanelTopMargin;

    // ── Background (rounded) ────────────────────────────────────────────
    sf::ConvexShape bg = makeRoundedRect({kPanelWidth, panelHeight}, kCornerRadius);
    bg.setPosition({panelX, panelY});
    bg.setFillColor(kBgColor);
    bg.setOutlineThickness(1.0f);
    bg.setOutlineColor(sf::Color(51, 65, 85, 200)); // slate-700
    window.draw(bg);

    // Decorative top accent line (rounded top corners only)
    sf::ConvexShape topAccent = makeRoundedRect({kPanelWidth, kCornerRadius * 2.0f}, kCornerRadius);
    topAccent.setPosition({panelX, panelY});
    topAccent.setFillColor(kHighlightBar);
    window.draw(topAccent);

    // Overlay a rect to flatten the bottom of the accent so it only shows the top rounded part
    sf::RectangleShape accentCover(sf::Vector2f(kPanelWidth - 2.0f, kCornerRadius));
    accentCover.setPosition({panelX + 1.0f, panelY + kCornerRadius});
    accentCover.setFillColor(kBgColor);
    window.draw(accentCover);

    // ── Title ───────────────────────────────────────────────────────────
    const float titleY = panelY + kPanelPadding;
    sf::Text titleText(*font, title, 13);
    titleText.setFillColor(kTitleColor);
    titleText.setStyle(sf::Text::Bold);
    titleText.setLetterSpacing(1.5f);
    titleText.setPosition({panelX + kPanelPadding, titleY});
    window.draw(titleText);

    // Title underline
    sf::RectangleShape titleLine(sf::Vector2f(kPanelWidth - kPanelPadding * 2.0f, 1.0f));
    titleLine.setPosition({panelX + kPanelPadding, titleY + 20.0f});
    titleLine.setFillColor(sf::Color(51, 65, 85, 120));
    window.draw(titleLine);

    // ── Code Lines ──────────────────────────────────────────────────────
    const float codeStartY = titleY + kTitleHeight;
    const float lineNumWidth = 28.0f;

    for (size_t i = 0; i < lines.size(); ++i) {
        const float lineY = codeStartY + static_cast<float>(i) * kLineHeight;

        // Don't render lines that overflow the panel
        if (lineY + kLineHeight > panelY + panelHeight - 4.0f) break;

        const bool isHighlighted = (static_cast<int>(i) == highlightedLine);

        // Highlight background
        if (isHighlighted) {
            sf::RectangleShape hlBg(sf::Vector2f(kPanelWidth - 2.0f, kLineHeight));
            hlBg.setPosition({panelX + 1.0f, lineY});
            hlBg.setFillColor(kHighlightBg);
            window.draw(hlBg);

            // Left accent bar
            sf::RectangleShape hlBar(sf::Vector2f(kHighlightBarWidth, kLineHeight));
            hlBar.setPosition({panelX, lineY});
            hlBar.setFillColor(kHighlightBar);
            window.draw(hlBar);
        }

        // Line number
        sf::Text lineNum(*font, std::to_string(i + 1), 12);
        lineNum.setFillColor(isHighlighted ? kHighlightBar : kLineNumberColor);
        lineNum.setPosition({panelX + kPanelPadding, lineY + 4.0f});
        window.draw(lineNum);

        // Code text
        sf::Text codeText(*font, lines[i], 14);
        codeText.setFillColor(isHighlighted ? kHighlightText : kLineColor);
        if (isHighlighted) {
            codeText.setStyle(sf::Text::Bold);
        }
        codeText.setPosition({panelX + kPanelPadding + lineNumWidth, lineY + 3.0f});
        window.draw(codeText);
    }
}

} // namespace UI
