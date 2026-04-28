#include "TrieScene.h"
#include "TrieVisualizer.h"
#include "PseudocodeData.h"
#include "ResourceManager.h"

TrieScene::TrieScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    auto* trieViz = new TrieVisualizer();
    setVisualizer(trieViz);
    displayStatus("Trie ready. Use Insert / Search / Delete.");
}

void TrieScene::onInsert(const std::string& value) {
    if (value.empty()) { displayStatus("Please enter a word to insert."); return; }
    auto* v = dynamic_cast<TrieVisualizer*>(visualizer.get());
    if (v) {
        if (pseudocodePanel)
            pseudocodePanel->setPseudocode(Pseudocode::kTrieInsertTitle,
                                           Pseudocode::kTrieInsert);
        v->insertWord(value);
        displayStatus("Inserting: \"" + value + "\"");
    }
}

void TrieScene::onSearch(const std::string& value) {
    if (value.empty()) { displayStatus("Please enter a word to search."); return; }
    auto* v = dynamic_cast<TrieVisualizer*>(visualizer.get());
    if (v) {
        if (pseudocodePanel)
            pseudocodePanel->setPseudocode(Pseudocode::kTrieSearchTitle,
                                           Pseudocode::kTrieSearch);
        v->searchWord(value);
        displayStatus("Searching: \"" + value + "\"");
    }
}

void TrieScene::onDelete(const std::string& value) {
    if (value.empty()) { displayStatus("Please enter a word to delete."); return; }
    auto* v = dynamic_cast<TrieVisualizer*>(visualizer.get());
    if (v) {
        if (pseudocodePanel)
            pseudocodePanel->setPseudocode(Pseudocode::kTrieDeleteTitle,
                                           Pseudocode::kTrieDelete);
        v->removeWord(value);
        displayStatus("Deleting: \"" + value + "\"");
    }
}

void TrieScene::onUpdate(const std::string& key, const std::string& value) {
    if (key.empty() || value.empty()) {
        displayStatus("Please enter both old and new words.");
        return;
    }
    auto* v = dynamic_cast<TrieVisualizer*>(visualizer.get());
    if (v) {
        // Check if key exists before attempting update
        if (!v->wordExists(key)) {
            displayStatus("Word \"" + key + "\" not found in trie!");
            return;
        }
        // Update = delete old + insert new; show insert pseudocode for the final step
        if (pseudocodePanel)
            pseudocodePanel->setPseudocode(Pseudocode::kTrieInsertTitle,
                                           Pseudocode::kTrieInsert);
        v->removeWord(key);
        v->insertWord(value);
        displayStatus("Updated \"" + key + "\" -> \"" + value + "\"");
    }
}

void TrieScene::onReset() {
    if (visualizer) visualizer->reset();
    if (pseudocodePanel) pseudocodePanel->hide();
    displayStatus("Trie reset.");
}

void TrieScene::onPlaybackSpeedChanged(float speed) {
    if (visualizer) visualizer->setPlaybackSpeed(speed);
}

void TrieScene::onTogglePlaybackMode(bool autoRun) {
    if (visualizer) visualizer->setAutoRun(autoRun);
    displayStatus(autoRun ? "Playback: auto" : "Playback: manual");
}

void TrieScene::onGoToFirstStep()    { if (visualizer) visualizer->goToFirstStep(); }
void TrieScene::onGoToPreviousStep() { if (visualizer) visualizer->goToPreviousStep(); }
void TrieScene::onGoToNextStep()     { if (visualizer) visualizer->goToNextStep(); }
void TrieScene::onGoToFinalStep()    { if (visualizer) visualizer->goToFinalStep(); }

std::string TrieScene::getSceneTitle() const {
    return "Trie Visualization";
}
