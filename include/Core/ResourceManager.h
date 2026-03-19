#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
	static ResourceManager &getInstance();
	// Resources retrieval
	sf::Texture &getTexture2D(const std::string &alias);
	sf::Font &getFont(const std::string &alias);

private:
	ResourceManager();
	~ResourceManager();

	ResourceManager(const ResourceManager &) = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;

	void _preLoadTexture2D(const std::string &filePath, const std::string &alias);
	void _preLoadFont(const std::string &filePath, const std::string &alias);

	void _unloadTexture2D(const std::string &alias);
	void _unloadFont(const std::string &alias);

	std::unordered_map<std::string, sf::Texture> _textures;
	std::unordered_map<std::string, sf::Font> _fonts;

	std::map<std::string, std::string> _MappingAliasToFilename;
};