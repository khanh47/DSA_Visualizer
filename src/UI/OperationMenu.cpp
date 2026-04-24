#include "OperationMenu.h"

#include "VisualizationScene.h"

namespace {
std::vector<OperationMenuItem> getOperationMenuItems(const std::string& sceneTitle) {
    if (sceneTitle == "Kruskal Visualization") {
        return {
            {"Random", OperationType::RANDOM},
            {"Run", OperationType::RUN},
            {"Reset", OperationType::RESET}
        };
    } else if (sceneTitle == "Linked List Visualization") {
        return {
            {"Insert", OperationType::INSERT},
            {"Delete", OperationType::DELETE},
            {"Search", OperationType::SEARCH},
            {"Update", OperationType::UPDATE}
        };
    } else {
        return {
            {"Insert", OperationType::INSERT},
            {"Delete", OperationType::DELETE},
            {"Search", OperationType::SEARCH},
            {"Update", OperationType::UPDATE},
            {"Reset", OperationType::RESET}
        };
    }
}
} // namespace

OperationMenu::OperationMenu(VisualizationScene& scene)
    : scene(scene),
      position({50.0f, 90.0f}),
      size({1500.0f, 60.0f}) {
    background.setPosition(position);
    background.setSize(size);
    background.setFillColor(sf::Color::Transparent);

    buildUI();
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

std::string OperationMenu::getInsertOption() const {
    if (insertTypeSelectBox) {
        return insertTypeSelectBox->getSelected();
    }
    return "At Tail";
}

std::size_t OperationMenu::getInputCount() const {
    return inputTextBoxes.size();
}

void OperationMenu::processEvents(const sf::Event& event) {
    if (menu) {
        menu->processEvent(event);
    }

    inputTextBoxes.processEvent(event);
    
    if (insertTypeSelectBox) {
        insertTypeSelectBox->processEvent(event);
    }
}

void OperationMenu::update(float deltaTime) {
    (void)deltaTime;
}

void OperationMenu::render(sf::RenderWindow& window) {
    if (menu) {
        menu->render(window);
    }

    inputTextBoxes.render(window);

    if (insertTypeSelectBox) {
        insertTypeSelectBox->render(window);
    }
}

void OperationMenu::buildUI() {
    menu = std::make_unique<UI::ButtonMenu>();
    inputTextBoxes.clear();

    menuItems = getOperationMenuItems(scene.getSceneTitle());
    const bool isLinkedList = scene.getSceneTitle() == "Linked List Visualization";

    const float buttonWidth = 120.0f;
    const float inputWidth = 90.0f;
    const float selectWidth = 130.0f;
    const float elementGap = 8.0f;
    const float inputGap = 20.0f;
    const float blockGap = isLinkedList ? 100.0f : 75.0f;
    const float height = 42.0f;
    const float currentY = position.y;

    float totalWidth = 0.0f;
    for (std::size_t i = 0; i < menuItems.size(); ++i) {
        const auto& item = menuItems[i];
        totalWidth += buttonWidth;

        if (item.type != OperationType::RANDOM &&
            item.type != OperationType::RUN &&
            item.type != OperationType::RESET) {
            
            totalWidth += inputGap;

            if (item.type == OperationType::UPDATE) {
                totalWidth += inputWidth * 2.0f + elementGap;
            } else if (item.type == OperationType::INSERT) {
                totalWidth += inputWidth;
                if (isLinkedList) {
                    totalWidth += elementGap + selectWidth;
                }
            } else {
                totalWidth += inputWidth;
            }
        }

        if (i < menuItems.size() - 1) {
            totalWidth += blockGap;
        }
    }

    float currentX = position.x + std::max(0.0f, (size.x - totalWidth) * 0.5f);

    for (const auto& item : menuItems) {
        auto button = std::make_shared<UI::Button>(
            sf::Vector2f(currentX, currentY),
            sf::Vector2f(buttonWidth, height),
            sf::Color(50, 66, 96),
            item.label,
            20
        );
        button->setCommand(createOperationCommand(&scene, this, item.type));
        menu->addButton(button);
        currentX += buttonWidth;

        if (item.type == OperationType::RANDOM ||
            item.type == OperationType::RUN ||
            item.type == OperationType::RESET) {
            currentX += blockGap;
            continue;
        }

        currentX += inputGap;

        if (item.type == OperationType::UPDATE) {
            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(currentX, currentY),
                sf::Vector2f(inputWidth, height),
                isLinkedList ? "Index" : "Old", 20, 6
            ));
            currentX += inputWidth + elementGap;

            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(currentX, currentY),
                sf::Vector2f(inputWidth, height),
                "New", 20, 6
            ));
            currentX += inputWidth;
        } else if (item.type == OperationType::INSERT) {
            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(currentX, currentY),
                sf::Vector2f(inputWidth, height),
                "Value", 20, 6
            ));
            currentX += inputWidth;

            if (isLinkedList) {
                currentX += elementGap;
                insertTypeSelectBox = std::make_unique<UI::SelectBox>(
                    sf::Vector2f(currentX, currentY),
                    sf::Vector2f(selectWidth, height),
                    std::vector<std::string>{"At Tail", "At Head"},
                    20
                );
                currentX += selectWidth;
            }
        } else {
            std::string placeholder = "Value";
            if (item.type == OperationType::DELETE && isLinkedList) {
                placeholder = "Index";
            }
            inputTextBoxes.add(std::make_unique<UI::TextBox>(
                sf::Vector2f(currentX, currentY),
                sf::Vector2f(inputWidth, height),
                placeholder, 20, 6
            ));
            currentX += inputWidth;
        }

        currentX += blockGap;
    }
}
