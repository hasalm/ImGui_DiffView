#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

        DrawSelection();
        DrawDiffView ();
        DrawStats    ();

    ImGui::End();
}

void WindowClass::DrawSelection()
{
	ImGui::InputText("Left",&filePathL);
	ImGui::SameLine();
	if(ImGui::Button("Save###Left"))
		SaveFileContent(filePathL,fileContentL);

	ImGui::InputText("Right",&filePathR);
	ImGui::SameLine();
	if(ImGui::Button("Save###Right"))
		SaveFileContent(filePathR,fileContentR);
if(ImGui::Button("Compare"))
{
	fileContentL = LoadFileContent(filePathL);
	fileContentR = LoadFileContent(filePathR);
	CreateDiff();
	}
}
void WindowClass::DrawDiffView()
{
constexpr static auto swap_width = 40.0F;
const auto parent_size = ImVec2(ImGui::GetContentRegionAvail().x, 500.0F);
const auto child_size =
ImVec2(parent_size.x / 2.0F - swap_width, parent_size.y);
const auto swap_size = ImVec2(swap_width, child_size.y);

ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
ImGui::BeginChild("Parent", parent_size, true);
if(ImGui::BeginChild("DiffL", child_size, false))
{
for(std::size_t i = 0; i < fileContentL.size(); ++i)
{
if(!diffResultL[i].empty())
{
ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),
"%s",
fileContentL[i].data());
}
else
{
ImGui::Text("%s",fileContentL[i].data());
}
}
}
ImGui::EndChild();
ImGui::SameLine();
const auto line_height = ImGui::GetTextLineHeightWithSpacing();
const auto button_size = ImVec2(15.0F, line_height);
if(ImGui::BeginChild("Swap",swap_size, true))
{
for(std::size_t i = 0; i < diffResultL.size(); i++)
{
const auto left_label = fmt::format("<##{}", i);
const auto right_label = fmt::format(">##{}", i);
		    if(!diffResultL[i].empty() ||  !diffResultR[i].empty())
{
if(ImGui::Button(left_label.data(),button_size))
{
					if(fileContentL.size() > i && fileContentR.size() > i)
						fileContentL[i] = fileContentR[i];
					else if(fileContentR.size() > i)
						fileContentL.push_back(fileContentR[i]);
					CreateDiff();
				}
ImGui::SameLine();
if(ImGui::Button(right_label.data(),button_size))
				{
					if(fileContentL.size() > i && fileContentR.size() > i)
						fileContentR[i] = fileContentL[i];
					else if(fileContentL.size() > i)
						fileContentR.push_back(fileContentL[i]);
CreateDiff();
}
}
else
{
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + line_height);
}
}
}
ImGui::EndChild();
ImGui::SameLine();
if(ImGui::BeginChild("Diff2", child_size, false))
{
for(std::size_t i = 0; i < fileContentR.size(); i++)
{
if(!diffResultR[i].empty())
{
ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),
"%s",
fileContentR[i].data());
}
else
{
ImGui::Text("%s", fileContentR[i].data());
}
}
}
ImGui::EndChild();
ImGui::EndChild();

ImGui::PopStyleVar();
}

void WindowClass::DrawStats() {

    auto diff_lines_count = std::size_t{0};
    for(const auto &line : diffResultL)
        {
            if(!line.empty())   ++diff_lines_count;
	    }
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20.0F);
    ImGui::Text("Diff lines count: %u", diff_lines_count);
}

WindowClass::FileContent WindowClass::LoadFileContent(std::string_view file_path)
{
auto content = FileContent{};
auto in = std::ifstream{file_path.data()};
if(in.is_open())
{
auto line = std::string{};
while(std::getline(in, line))
	content.push_back(line);
	}
in.close();

return content;
}
void WindowClass::SaveFileContent(std::string_view file_path,
									FileContent &file_content)
									{
									auto out = std::ofstream{file_path.data()};
if(out.is_open())
{
for(const auto &line : file_content)
	out << line << '\n';
	}
out.close();
}
void WindowClass::CreateDiff()
{
diffResultL.clear();
diffResultR.clear();
const auto max_num_lines =
	std::max(fileContentL.size(),fileContentR.size());
for(std::size_t i = 0; i < max_num_lines;++i)
{
const auto lineL = i < fileContentL.size() ? fileContentL[i] : "EMPTY";
const auto lineR = i < fileContentR.size() ? fileContentR[i]: "EMPTY";
if(lineL != lineR)
{
diffResultL.push_back(lineL);
diffResultR.push_back(lineR);
}
else
{
diffResultL.push_back("");
diffResultR.push_back("");
}
}
}
void render(WindowClass &window_obj)
{
    window_obj.Draw("Diff Viewer ");
}
