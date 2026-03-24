#include "OperationPanel.h"
#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include "OperationSchema.h"
#include <algorithm>
#include <cctype>

OperationPanel::OperationPanel(VisualizationScene& scene)
    : BasePanel(scene, {1350.0f, (1080.0f - 600.0f) * 0.5f}, {500.0f, 600.0f}),
    scene(scene) {
    if (scene.getSceneTitle() == "Graph Visualization")
        type = OperationType::VERTEX;
    else
        type = OperationType::INSERT;
    panelBackground.setPosition(position);
    panelBackground.setSize(size);
    panelBackground.setFillColor(sf::Color(42, 51, 70, 200));

    initOperationMenu();
    initInputBoxes();
    initRunMenu();
}

float OperationPanel::calculateMenuStartX(std::size_t buttonCount, float spacing, float buttonWidth) const {
    if (buttonCount == 0) {
        return position.x;
    }

    const float totalWidth = buttonWidth + static_cast<float>(buttonCount - 1) * spacing;
    return position.x + (size.x - totalWidth) * 0.5f;
}

void OperationPanel::get(const OperationType& _type) {
    type = _type;
    initInputBoxes();
    initRunMenu();
}

std::string OperationPanel::getInputValue(std::size_t index) const {
    return inputTextBoxes.getText(index);
}

std::size_t OperationPanel::getInputCount() const {
    return inputTextBoxes.size();
}

std::string OperationPanel::getRunAlgorithm() const {
    if (!algorithmSelector) {
        return "prim";
    }

    std::string selected = algorithmSelector->getSelected();
    std::transform(selected.begin(), selected.end(), selected.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return selected;
}

void OperationPanel::processEvents(const sf::Event& event) {
    if (operationMenu) {
        operationMenu->processEvent(event);
    }

    inputTextBoxes.processEvent(event);

    if (algorithmSelector) {
        algorithmSelector->processEvent(event);
    }

    if (runMenu) {
        runMenu->processEvent(event);
    }
}

void OperationPanel::update(float deltaTime) {
}

void OperationPanel::render(sf::RenderWindow& window) {
    window.draw(panelBackground);

    if (operationMenu) {
        operationMenu->render(window);
    }

    for (const sf::Text& inputLabel : inputLabels) {
        window.draw(inputLabel);
    }

    inputTextBoxes.render(window);

    if (algorithmSelectorLabel) {
        window.draw(*algorithmSelectorLabel);
    }

    if (algorithmSelector) {
        algorithmSelector->render(window);
    }

    if (runMenu) {
        runMenu->render(window);
    }
}

void OperationPanel::initInputBoxes() {
    algorithmSelector.reset();
    algorithmSelectorLabel.reset();
    inputLabels.clear();
    inputTextBoxes.clear();

    const OperationSchema schema = getOperationSchema(type);
    const std::vector<InputFieldDef>& inputFields = schema.inputFields;
    const sf::Vector2f inputSize(size.x - 80.0f, 52.0f);
    const float inputStartX = position.x + 40.0f;
    const float labelToInputOffset = 28.0f;
    const float operationMenuTop = position.y + 22.0f;
    const float operationMenuHeight = 44.0f;
    const float runMenuTop = position.y + size.y - 100.0f;
    const float contentTop = operationMenuTop + operationMenuHeight + 28.0f;
    const float contentBottom = runMenuTop - 24.0f;
    const float availableHeight = std::max(0.0f, contentBottom - contentTop);

    sf::Font& font = ResourceManager::getInstance().getFont("Roboto");

    float inputStartY = contentTop;
    float inputRowGap = 24.0f;
    if (!inputFields.empty()) {
        const float blockHeight = labelToInputOffset + inputSize.y;
        if (inputFields.size() > 1) {
            const float maxGap = (availableHeight - blockHeight * static_cast<float>(inputFields.size())) /
                                 static_cast<float>(inputFields.size() - 1);
            inputRowGap = std::max(8.0f, std::min(28.0f, maxGap));
        } else {
            inputRowGap = 0.0f;
        }

        const float totalBlockHeight =
            blockHeight * static_cast<float>(inputFields.size()) +
            inputRowGap * static_cast<float>(inputFields.size() > 0 ? inputFields.size() - 1 : 0);
        inputStartY = contentTop + std::max(0.0f, (availableHeight - totalBlockHeight) * 0.5f);
    }

    for (std::size_t i = 0; i < inputFields.size(); ++i) {
        const float blockHeight = labelToInputOffset + inputSize.y;
        const float rowStride = blockHeight + inputRowGap;
        const float blockY = inputStartY + static_cast<float>(i) * rowStride;

        sf::Text inputLabel(font, inputFields[i].label, 18);
        inputLabel.setFillColor(sf::Color::White);
        inputLabel.setPosition({inputStartX, blockY});
        inputLabels.push_back(inputLabel);

        inputTextBoxes.add(std::make_unique<UI::TextBox>(
            sf::Vector2f(inputStartX, blockY + labelToInputOffset),
            inputSize,
            inputFields[i].placeholder,
            20,
            40
        ));
    }

    if (schema.useSelector) {
        const float selectorBlockHeight = labelToInputOffset + 52.0f;
        const float selectorStartY = contentTop + std::max(0.0f, (availableHeight - selectorBlockHeight) * 0.5f);

        algorithmSelectorLabel = std::make_unique<sf::Text>(font, schema.selectorLabel, 18);
        algorithmSelectorLabel->setFillColor(sf::Color::White);
        algorithmSelectorLabel->setPosition({position.x + 40.0f, selectorStartY});

        algorithmSelector = std::make_unique<UI::SelectBox>(
            sf::Vector2f(position.x + 40.0f, selectorStartY + labelToInputOffset),
            sf::Vector2f(size.x - 80.0f, 52.0f),
            schema.selectorOptions,
            20
        );
    }
}
void OperationPanel::initOperationMenu() {
    operationMenu = std::make_unique<UI::ButtonMenu>();

    const float buttonWidth = 80.0f;
    const float buttonHeight = 44.0f;
    const float spacing = 100.0f;

    const bool isGraphScene = scene.getSceneTitle() == "Graph Visualization";
    const std::vector<OperationMenuItem> menuItems = getOperationMenuItems(isGraphScene);
    const std::size_t buttonCount = menuItems.size();
    const float startX = calculateMenuStartX(buttonCount, spacing, buttonWidth);
    const float startY = position.y + 22.0f;

    operationMenu->setLayoutProperties(
        {startX, startY},
        {buttonWidth, buttonHeight},
        spacing,
        true,
        sf::Color(100, 149, 237),
        18 
    );

    for (const OperationMenuItem& item : menuItems) {
        operationMenu->addButtonAuto(item.label, createOperationCommand(this, item.type));
    }
}

void OperationPanel::initRunMenu() {
    runMenu = std::make_unique<UI::ButtonMenu>();

    const bool singleRunButton =
        type == OperationType::VERTEX ||
        type == OperationType::EDGE ||
        type == OperationType::RANDOM;

    const float buttonWidth = 200.0f;
    const float buttonHeight = 44.0f;
    const float spacing = 220.0f;
    const std::size_t buttonCount = singleRunButton ? 1 : 2;
    const float startX = calculateMenuStartX(buttonCount, spacing, buttonWidth);
    const float startY = position.y + size.y - 100.0f;

    runMenu->setLayoutProperties(
        {startX, startY},
        {buttonWidth, buttonHeight},
        spacing,
        true,
        sf::Color(100, 149, 237),
        18 
    );

    if (singleRunButton) {
        runMenu->addButtonAuto("Run", createRunCommand(&scene, this, type));
    } else {
        runMenu->addButtonAuto("Run step by step", createRunCommand(&scene, this, type));
        runMenu->addButtonAuto("Run instantly", createRunCommand(&scene, this, type));
    }
}