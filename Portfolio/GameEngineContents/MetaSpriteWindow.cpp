#include "PreCompile.h"
#include "MetaSpriteWindow.h"
#include <GameEngineCore/GameEngineFolderTexture.h>
#include <GameEngineCore/GameEngineCoreDebug.h>

MetaSpriteWindow::MetaSpriteWindow()
    : CurMetaFolder_{}
    , CurAtlasFolder_{}
    , CurMetaFile_{}
    , CurAtlasFile_{}
{}

MetaSpriteWindow::~MetaSpriteWindow() {}

void MetaSpriteWindow::Initialize(GameEngineLevel* _Level) {}

void MetaSpriteWindow::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
    AtlasFileButton();
    MetaFileButton();

    // AtlasFileButton();
    // MetaFileButton();
}


void MetaSpriteWindow::MetaFileButton()
{
    if (nullptr == TargetTexture)
    {
        return;
    }


    if (true == ImGui::Button("MetaFileLoad"))
    {
        std::string Path
            = GameEngineGUI::OpenFileDlg(GameEngineString::AnsiToUTF8Return("메타 파일 로드"), MetaDir.GetFullPath());

        if (false == Path.empty())
        {
            GameEngineFile LoadFile = GameEngineFile(Path);
            LoadFile.Open(OpenMode::Read, FileMode::Text);

            std::string AllText = LoadFile.GetString();

            size_t StartPos = 0;

            while (true)
            {
                StartPos = AllText.find("rect:", StartPos);

                if (StartPos == std::string::npos)
                {
                    break;
                }

                size_t EndPos = AllText.find("outline:", StartPos);

                if (EndPos == std::string::npos)
                {
                    break;
                }

                std::string CutDataString = AllText.substr(StartPos, EndPos - StartPos);

                int StartX = 0;
                int StartY = 0;
                int SizeX  = 0;
                int SizeY  = 0;

                {
                    std::string FindString   = "x:";
                    size_t      DataStartPos = CutDataString.find(FindString);
                    size_t      DataEndpos   = CutDataString.find("\n", DataStartPos);
                    std::string XString      = CutDataString.substr(DataStartPos + FindString.size(),
                                                               DataEndpos - DataStartPos + FindString.size());
                    StartX                   = atoi(XString.c_str());
                }

                {
                    std::string FindString   = "y:";
                    size_t      DataStartPos = CutDataString.find(FindString);
                    size_t      DataEndpos   = CutDataString.find("\n", DataStartPos);
                    std::string XString      = CutDataString.substr(DataStartPos + FindString.size(),
                                                               DataEndpos - DataStartPos + FindString.size());
                    StartY                   = atoi(XString.c_str());
                }

                {
                    std::string FindString   = "width:";
                    size_t      DataStartPos = CutDataString.find(FindString);
                    size_t      DataEndpos   = CutDataString.find("\n", DataStartPos);
                    std::string XString      = CutDataString.substr(DataStartPos + FindString.size(),
                                                               DataEndpos - DataStartPos + FindString.size());
                    SizeX                    = atoi(XString.c_str());
                }

                {
                    std::string FindString   = "height:";
                    size_t      DataStartPos = CutDataString.find(FindString);
                    size_t      DataEndpos   = CutDataString.find("\n", DataStartPos);
                    std::string XString      = CutDataString.substr(DataStartPos + FindString.size(),
                                                               DataEndpos - DataStartPos + FindString.size());
                    SizeY                    = atoi(XString.c_str());
                }

                TargetTexture->Cut(StartX, StartY, SizeX, SizeY);

                StartPos += 1;
                int a = 0;
            }

            // GameEngineDebug::DrawTexture(TargetTexture, {0,0});

            for (size_t i = 0; i < TargetTexture->GetCutCount(); i++)
            {}


            // TargetTexture->Cut();


            // CurMetaFolder_ = GameEnginePath::GetFileName(Path);
            // GameEngineMetaDataFolder::Load(Path.c_str());
        }
    }

    if (false == CurMetaFolder_.empty())
    {
        ImGui::SameLine();
        ImGui::Text(CurMetaFolder_.c_str());
    }
}

void MetaSpriteWindow::AtlasFileButton()
{
    if (true == ImGui::Button("AtlasFileLoad"))
    {
        std::string Path = GameEngineGUI::OpenFileDlg(GameEngineString::AnsiToUTF8Return("아틀라스 시트 로드"),
                                                      MetaDir.GetFullPath());

        if (false == Path.empty())
        {
            TargetTexture = GameEngineTexture::Load(Path);
        }
    }

    if (false == CurAtlasFolder_.empty())
    {
        ImGui::SameLine();
        ImGui::Text(TargetTexture->GetNameCopy().c_str());
    }
}