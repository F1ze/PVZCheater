#include "gui.h"

PVZService *pvzServ = new PVZService;

void gui::init() {
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED, wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 3000, 3000, nullptr, nullptr, wc.hInstance, nullptr);


    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    auto bgColor = ImColor(0,0,0);
    ImVec4 clear_color = ImVec4(bgColor);
    SetLayeredWindowAttributes(hwnd, bgColor, NULL, LWA_COLORKEY);


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        mainGui();

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    delete pvzServ;

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}


// Helper functions
bool gui::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void gui::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void gui::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void gui::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void mainGui() 
{
    // window config
    static bool no_close = true;
    static bool no_resize = true;

    // state
    static int selectedGameType = 0;
    static float alpha = 1.0;
    static auto io = ImGui::GetIO();
    static LPRECT rect = nullptr;

    // game state
    static int sunCount = 100;

    // Close button in top-right
    if (!no_close) {
        ::PostQuitMessage(0);
        return;
    }

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;		// ImGuiWindowFlags_NoBackground
    if (no_resize) window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(550, -1));
    if (rect != nullptr) ImGui::SetNextWindowPos(ImVec2(rect->right, rect->top));
    else ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("PVZ Cheater", &no_close, window_flags);
    //mainMenu();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    if (pvzServ->IsGameRunning()) {
        ImGui::Text("Game is Running: %d", pvzServ->GetPid());
        rect = pvzServ->GetWndRect();


        if (ImGui::CollapsingHeader("Basic Info", 1))
        {
            static bool SunCntNotDecrease = false;
            static bool autoSunCollect = false;
            static bool plantNoCD = false;
            static bool plantCasually = false;
            static bool noPause = false;
            static bool zombieFreeze = false;
            static bool seckillBullet = false;

            ImGui::SeparatorText("Basic Part");

            if (ImGui::Checkbox("##1", &SunCntNotDecrease)) pvzServ->ToggleSunNotDecrease(SunCntNotDecrease);
            // Sun Part
            ImGui::SetItemTooltip("Sun not decrease.");
            ImGui::SameLine();
            if (ImGui::DragInt("Sun", &sunCount, 25, 0, 8000)) pvzServ->SetSunCount(sunCount);
            else sunCount = pvzServ->GetSunCount();

            if (ImGui::BeginTable("CheckTable", 3))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Checkbox("Auto Collect", &autoSunCollect)) pvzServ->ToggleAutoCollect(autoSunCollect);
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Checkbox("No Pause", &noPause)) pvzServ->ToggleNoPause(noPause);
                ImGui::TableSetColumnIndex(2);
                ImGui::EndTable();
            }


            ImGui::SeparatorText("Plant Part");

            {
                static int p[3] = {0, 7, 0x17};
                ImGui::InputInt3("row-col-code", p); ImGui::SameLine();
                if (ImGui::Button("Plant")) pvzServ->AddPlant(p[0], p[1], p[2]);
            }

            if (ImGui::BeginTable("CheckTable", 3))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Checkbox("Plant No CD", &plantNoCD)) pvzServ->TogglePlantNoCD(plantNoCD);
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Checkbox("Plant Casually", &plantCasually)) pvzServ->TogglePlantAnywhere(plantCasually);
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Checkbox("Seckill bullet", &seckillBullet)) pvzServ->ToggleSeckillBullet(seckillBullet);
                ImGui::EndTable();
            }

            ImGui::SeparatorText("Zombie Part");

            {
                static int p[2] = {0, 2};
                ImGui::InputInt2("row/code", p); ImGui::SameLine();
                if (ImGui::Button("Add")) pvzServ->AddZombie(p[0], p[1]);
            }

            if (ImGui::BeginTable("CheckTable", 3))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Checkbox("Zombie Freeze", &zombieFreeze)) pvzServ->ToggleZombieFreeze(zombieFreeze);
                ImGui::EndTable();
            }

            if (ImGui::BeginTable("CheckTable", 5))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Button("All Freeze", ImVec2(-1, 0))) pvzServ->FreezeAllZombie();
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button("All Kill", ImVec2(-1, 0))) pvzServ->KillAllZombie();

                ImGui::EndTable();
            }
        }

        if (ImGui::CollapsingHeader("Draw Info"))
        {
            static bool showZombieRect = false;
            ImGui::Checkbox("Show Zombie Line", &showZombieRect);
            if (showZombieRect) {
                auto draw = ImGui::GetForegroundDrawList();
                draw->AddRect(ImVec2(rect->left, rect->top), ImVec2(rect->right, rect->bottom), ImColor(0, 0, 255));
            }
        }


    }
    else
    {
        ImGui::Text("Game Not Running");
        rect = nullptr;
    }
    ImGui::End();
}