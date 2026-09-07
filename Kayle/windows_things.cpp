#include "windows_things.hpp"

// Fonction native Windows pour ouvrir la boîte de dialogue
std::string openFileDialog(HWND parentWindow) {
    std::string filePath = "";

    // Initialisation du composant COM
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return filePath;

    IFileOpenDialog* pFileOpen = nullptr;

    // Création de l'instance IFileOpenDialog
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        // Optionnel : Définir des filtres de fichiers (ex: Fichiers texte, Tous les fichiers)
        COMDLG_FILTERSPEC fileTypes[] = {
            { L"Tous les fichiers", L"*.*" },
            { L"Images", L"*.png;*.jpg;*.bmp" },
            { L"Fichiers texte", L"*.txt" }
        };
        pFileOpen->SetFileTypes(3, fileTypes);

        // Afficher la fenêtre d'exploration
        hr = pFileOpen->Show(parentWindow);

        // Si l'utilisateur a sélectionné un fichier
        if (SUCCEEDED(hr)) {
            IShellItem* pItem = nullptr;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath = nullptr;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (SUCCEEDED(hr)) {
                    // Conversion de WCHAR (UTF-16) vers std::string
                    std::wstring ws(pszFilePath);
                    filePath = std::string(ws.begin(), ws.end());
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }
    CoUninitialize();

    return filePath;
}