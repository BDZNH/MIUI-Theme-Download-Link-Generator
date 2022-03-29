// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filedlg.h>
#include <wx/gauge.h>
#include <wx/clipbrd.h>
#include <wx/notifmsg.h>
#include <wx/aboutdlg.h>

#include "Downloader.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame,public DownloadCallBack
{
public:
    MyFrame(const wxString& title);
private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    virtual void onMessage(const std::string& str, MESSAGE_TYPE type) override;
    virtual void onProgress(size_t total, size_t current) override;
    virtual void onStart() override;
    virtual void onFinished() override;
    virtual FILE* onSave(std::string filename) override;
    virtual void onURLChanged() override;

    wxDECLARE_EVENT_TABLE();

    wxPanel* mPanel = nullptr;
    wxBoxSizer* mMainlayoutContainer = nullptr;
    wxStaticText* mThemeUrlHint = nullptr;
    wxTextCtrl* mThemeUrl = nullptr;
    wxStaticText* mThemeDownloadUrlHint = nullptr;
    wxTextCtrl* mThemeDownloadUrl = nullptr;
    wxRadioBox* mVersionSelector = nullptr;
    wxButton* openStore = nullptr;
    wxButton* getDownloadLink = nullptr;
    wxButton* copyDownloadLink = nullptr;
    wxButton* downloadThemeAsFile = nullptr;
    wxGauge* mGauge = nullptr;
    Downloader* mDownloader = nullptr;
    FILE* mThemeFile = nullptr;
    bool downloadLinkValid = false;
    std::string mVersion = "V12";
};
enum
{
    ID_RADIOBUTTON_GROUP = 1,
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("MIUI主题下载器");
    frame->Show(true);
    frame->SetMinClientSize(frame->FromDIP(wxSize(480, 220)));
    frame->SetMaxClientSize(frame->FromDIP(wxSize(480, 220)));
    return true;
}
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title,wxDefaultPosition,{500,240})
{
    SetIcon(wxICON(appicon));
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT,"退出...\tCtrl+Q");
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT,"关于...\tCtrl+H");
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&文件");
    menuBar->Append(menuHelp, "&帮助");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("欢迎使用MIUI主题下载器");
    
    mPanel = new wxPanel(this,wxID_ANY,wxDefaultPosition,{500,240});
    mDownloader = new Downloader(this);
    mMainlayoutContainer = new wxBoxSizer(wxVERTICAL);
    mThemeUrlHint = new wxStaticText(mPanel, wxID_ANY, "MIUI主题链接", wxDefaultPosition, wxDefaultSize);
    mThemeUrl = new wxTextCtrl(mPanel, wxID_ANY, "", wxDefaultPosition, {460,-1}, wxTE_PROCESS_ENTER);
    mThemeUrl->Bind(wxEVT_TEXT_ENTER, [&](wxCommandEvent& event) {
        downloadLinkValid = false;
        mDownloader->tryget(mThemeUrl->GetLineText(0).ToStdString(), GET_DOWNLOAD_LINK_URL,mVersion);
        });
    mThemeDownloadUrlHint = new wxStaticText(mPanel, wxID_ANY, "主题下载链接", wxDefaultPosition, wxDefaultSize);
    mThemeDownloadUrl = new wxTextCtrl(mPanel, wxID_ANY, "", wxDefaultPosition, {460,-1}, wxTE_READONLY);

    mMainlayoutContainer->Add(mThemeUrlHint, wxSTRETCH_NOT, wxALIGN_LEFT | wxLEFT|wxTOP | wxRIGHT, 10);
    mMainlayoutContainer->Add(mThemeUrl, wxSTRETCH_NOT, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    mMainlayoutContainer->AddSpacer(7);
    mMainlayoutContainer->Add(mThemeDownloadUrlHint, wxSTRETCH_NOT, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    mMainlayoutContainer->Add(mThemeDownloadUrl, wxSTRETCH_NOT, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    wxArrayString choice;
    choice.Add("V4");
    choice.Add("V5");
    choice.Add("V6/V7  ");
    choice.Add("V8/V9  ");
    choice.Add("V10");
    choice.Add("V11");
    choice.Add("V12");
    mVersionSelector = new wxRadioBox(mPanel, ID_RADIOBUTTON_GROUP, "", wxDefaultPosition, wxDefaultSize, choice);
    mVersionSelector->SetSelection(choice.size()-1);
    mVersionSelector->Bind(wxEVT_RADIOBOX, [&](wxCommandEvent& event) {
        std::string version = mVersionSelector->GetString(mVersionSelector->GetSelection()).ToStdString();
        if (version == "V6/V7  ") 
        {
            mVersion = "V6";
        }
        else if (version == "V8/V9  ") 
        {
            mVersion = "V8";
        }
        else
        {
            mVersion = version;
        }
        });
    mMainlayoutContainer->Add(mVersionSelector, wxSTRETCH_NOT, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);

    wxBoxSizer* buttonsContainer = new wxBoxSizer(wxHORIZONTAL);
    openStore = new wxButton(mPanel, wxID_ANY, "打开主题商店");
    getDownloadLink = new wxButton(mPanel, wxID_ANY, "获取下载链接");
    copyDownloadLink = new wxButton(mPanel, wxID_ANY, "复制下载链接");
    downloadThemeAsFile = new wxButton(mPanel, wxID_ANY, "下载");
    openStore->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        ShellExecute(NULL, L"open", L"http://zhuti.xiaomi.com/", NULL, NULL, SW_SHOWNORMAL);
        });

    getDownloadLink->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        mDownloader->tryget(mThemeUrl->GetLineText(0).ToStdString(), GET_DOWNLOAD_LINK_URL, mVersion);
        });

    copyDownloadLink->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        if (!downloadLinkValid)
        {
            mDownloader->tryget(mThemeUrl->GetLineText(0).ToStdString(), GET_DOWNLOAD_LINK_URL, mVersion);
            if (!downloadLinkValid) {
                return;
            }
        }
        wxClipboard wxclipboard;
        if (wxclipboard.Open())
        {
            wxclipboard.SetData(new wxTextDataObject(mThemeDownloadUrl->GetLineText(0)));
            wxclipboard.Close();
            wxNotificationMessage notification("拷贝成功", mThemeDownloadUrl->GetLineText(0),mPanel);
            // auto hide
            notification.Show(-1);
        }
        });

    downloadThemeAsFile->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        if (!downloadLinkValid)
        {
            mDownloader->tryget(mThemeUrl->GetLineText(0).ToStdString(), GET_DOWNLOAD_LINK_URL, mVersion);
            if (!downloadLinkValid) {
                return;
            }
        }
        mDownloader->tryget(mThemeDownloadUrl->GetLineText(0).ToStdString(), DOWNLOAD_THEME_AS_FILE, mVersion);
        });

    buttonsContainer->Add(openStore, 2);
    buttonsContainer->Add(getDownloadLink, 2, wxLEFT, 2);
    buttonsContainer->Add(copyDownloadLink, 2, wxLEFT, 2);
    buttonsContainer->Add(downloadThemeAsFile, 1,wxLEFT,2);
    mMainlayoutContainer->AddSpacer(7);
    mMainlayoutContainer->Add(buttonsContainer, wxSTRETCH_NOT,  wxLEFT | wxRIGHT, 10);

    mGauge = new wxGauge(mPanel, wxID_ANY, 100, wxDefaultPosition, { 460,-1 }, wxGA_HORIZONTAL);
    mMainlayoutContainer->AddSpacer(7);
    mMainlayoutContainer->Add(mGauge, wxSTRETCH_NOT, wxLEFT | wxBOTTOM | wxRIGHT, 11);

    mPanel->SetSizerAndFit(mMainlayoutContainer);
    Center();
    DoLayout();
    //disableMaxMum
    //SetWindowStyle(GetWindowStyle() ^ wxMAXIMIZE_BOX);
    //Fit();
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("关于本程序");
    aboutInfo.SetDescription(_("小米主题离线下载器"));
    aboutInfo.SetCopyright("(C) 2018-2022");
    aboutInfo.SetWebSite("https://github.com/BDZNH/");
    aboutInfo.AddDeveloper("BDZNH");
    wxAboutBox(aboutInfo);
}
void MyFrame::onMessage(const std::string& str, MESSAGE_TYPE type)
{
    switch (type)
    {
    case GET_LINK_SUCCESS:
        mThemeDownloadUrl->SetLabel(str);
        downloadLinkValid = true;
        break;
    case GET_LINK_FAILED:
        wxMessageBox(str, "警告", wxOK | wxICON_WARNING);
        break;
    case DOWNLAOD_SUCCESS:
        wxMessageBox("下载成功", "警告", wxOK | wxICON_INFORMATION);
        break;
    case DOWNLOAD_FAILED:
        {wxMessageBox(str, "警告", wxOK | wxICON_WARNING); }
        break;
    case INVALID_MIUI_THEME_URL:
    {
        std::string msg;
        if (str == "") {
            msg = "请输入主题链接";
        }
        else 
        {
            msg = "\"" + str + "\"不是有效的主题链接";
        }
        wxMessageBox(msg, "警告", wxOK | wxICON_WARNING);
    }
        break;
    default:
        break;
    }
    
}
void MyFrame::onProgress(size_t total, size_t current)
{
    if (total != 0) {
        // 应该不会溢出吧，毕竟只是一个主题包
        mGauge->SetValue((current >= total) ? 100 : (current * 100 / total));
    }
}
void MyFrame::onStart()
{
    openStore->Disable();
    getDownloadLink->Disable();
    copyDownloadLink->Disable();
    downloadThemeAsFile->Disable();
}
void MyFrame::onFinished()
{
    if (mThemeFile)
    {
        fclose(mThemeFile);
        mThemeFile = nullptr;
    }
    openStore->Enable();
    getDownloadLink->Enable();
    copyDownloadLink->Enable();
    downloadThemeAsFile->Enable();
    mGauge->SetValue(0);
}

FILE* MyFrame::onSave(std::string filename)
{
    std::string filenameWithoutExtract = filename.substr(0, filename.find_first_of('.'));
    wxFileDialog saveFileDialog(this, filename, "./", filenameWithoutExtract, "(*.mtz)|*.mtz", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return nullptr;
    }
    mThemeFile = fopen(saveFileDialog.GetPath(),"wb+");
    if (!mThemeFile)
    {
        wxMessageBox("无效的保存位置", "警告", wxOK | wxICON_WARNING);
        return nullptr;
    }
    return mThemeFile;
}

void MyFrame::onURLChanged()
{
    mThemeDownloadUrl->Clear();
    downloadLinkValid = false;
}

