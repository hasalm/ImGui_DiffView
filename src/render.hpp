#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <imgui.h>
class WindowClass
{
public:
    using FileContent = std::vector<std::string>;

public:
	WindowClass()
	: filePathL("textL.txt"), filePathR("textR.txt"),fileContentL({}),
	fileContentR({}), diffResultL({}),diffResultR({}){};


    void Draw(std::string_view label);

private:
    void DrawSelection();
    void DrawDiffView ();
    void DrawStats    ();

    FileContent LoadFileContent(std::string_view file_path);
    void        SaveFileContent(std::string_view file_path,FileContent &file_content);
    void        CreateDiff();
private:
    std::string filePathL;
    std::string filePathR;

    FileContent fileContentL;
    FileContent fileContentR;
    FileContent diffResultL ;
    FileContent diffResultR ;

};

void render(WindowClass &window_obj);

