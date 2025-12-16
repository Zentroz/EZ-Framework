#pragma once

#include<filesystem>

namespace fs = std::filesystem;

namespace ENGINE {

	class Project {
	public:
		Project() = default;

		bool CreateProject(std::string projectName, fs::path directory);
		bool OpenProject(fs::path directory);
		fs::path GetDirectory() { return m_Directory; }
		fs::path GetAssetDirectory() { return m_Directory / "Assets"; }
		fs::path GetLibraryDirectory() { return m_Directory / "Library"; }
		bool IsLoaded() { return isLoaded; }
	private:
		fs::path m_Directory;
		bool isLoaded = false;
	};

}