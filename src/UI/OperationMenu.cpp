#include "OperationMenu.h"

#include "VisualizationScene.h"

namespace {
std::vector<OperationMenuItem> getOperationMenuItems(bool isKruskalScene) {
    if (!isKruskalScene) {
        return {
            {"Insert", OperationType::INSERT},
            {"Delete", OperationType::DELETE},
            {"Search", OperationType::SEARCH},
            {"Update", OperationType::UPDATE},
            {"Reset", OperationType::RESET}
        };
    }

    return {
        {"Random", OperationType::RANDOM},
        {"Run", OperationType::RUN},
        {"Reset", OperationType::RESET}
    };
}
} // namespace

OperationMenu::OperationMenu(VisualizationScene& scene)
    : scene(scene),
      position({50.0f, 90.0f}),
      size({1300.0f, 60.0f}) {
    background.setPosition(position);
    background.setSize(size);
    background.setFillColor(sf::Color::Transparent);

    initMenu();
    initInputBoxes();
}

float OperationMenu::calculateMenuStartX(std::size_t buttonCount, float spacing, float buttonWidth) const {
    if (buttonCount == 0) {
        return position.x;
    }

    const float totalWidth = buttonWidth + static_cast<float>(buttonCount - 1) * spacing;
    return position.x + (size.x - totalWidth) * 0.5f;
}

std::string OperationMenu::getInputValue(std::size_t index) const {
    return inputTextBoxes.getText(index);
}

std::size_t OperationMenu::getInputCount() const {
    return inputTextBoxes.size();
}

void OperationMenu::processEvents(const sf::Event& event) {
    if (menu) {
        menu->processEvent(event);
    }

    inputTextBoxes.processEvent(event);
}

void OperationMenu::update(float deltaTime) {
    (void)deltaTime;
}

void OperationMenu::render(sf::RenderWindow& window) {
    if (menu) {
        menu->render(window);
    }

    inputTextBoxes.render(window);
}

void OperationMenu::initInputBoxes() {
    inputTextBoxes.clear();

    const float buttonWidth = 120.0f;
    const float slotSpacing = 350.0f;
    const float inputGap = 20.0f;
    const float inputY = position.y;
    const sf::Vector2f inputSize(90.0f, 42.0f);

    for (std::size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i].type == OperationType::RANDOM ||
            menuItems[i].type == OperationType::RUN ||
            menuItems[i].type == OperationType::RESET) {
            continue;
        }

        const float baseX = position.x + static_cast<float>(i) * slotSpacing + buttonWidth + inputGap;

        if (menuItems[i].type == OperationType::UPDATE) {
            const float splitGap = 8.0f;
            const float keyX = baseX;
            const float valueX = keyX + inputSize.x + splitGap;

            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(keyX, inputY),
                inputSize,
                "",
                20,
                6
            ));

            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(valueX, inputY),
                inputSize,
                "",
                20,
                6
            ));
            continue;
        }

        inputTextBoxes.add(std::make_unique<UI::TextBox>(
            sf::Vector2f(baseX, inputY),
            inputSize,
            "",
            20,
            6
        ));
    }
}

void OperationMenu::initMenu() {
    menu = std::make_unique<UI::ButtonMenu>();

    const float buttonWidth = 120.0f;
    const float buttonHeight = 42.0f;

    const bool isKruskalScene = scene.getSceneTitle() == "Kruskal Visualization";
    const float spacing = isKruskalScene ? 150.0f : 350.0f;
    menuItems = getOperationMenuItems(isKruskalScene);
    const float startX = position.x;
    const float startY = position.y;

    menu->setLayoutProperties(
        {startX, startY},
        {buttonWidth, buttonHeight},
        spacing,
        true,
        sf::Color(50, 66, 96),
        20
    );

    for (const OperationMenuItem& item : menuItems) {
        menu->addButtonAuto(item.label, createOperationCommand(&scene, this, item.type));
    }
}
