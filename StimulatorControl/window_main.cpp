#include<d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include<string>
#include<vector>
#include<array>
#include<fstream>

template<typename T>
class ComPtr {
	T* ptr_;
public:
	ComPtr():ptr_(nullptr){}
	~ComPtr() { if (ptr_) { ptr_->Release(); } }
	T* operator->() { return ptr_; }
	T** operator&() { return &ptr_; }
	operator T*() { return ptr_; }
	operator void**() { return reinterpret_cast<void**>(&ptr_); }
	void Release() { if (ptr_) { ptr_->Release(); ptr_ = nullptr; } }
};

class WaveWriter :protected std::ofstream {
public:
	WaveWriter(const char* file_name) :std::ofstream(file_name) { 
		*this << "fixed" << std::endl;
	}
	WaveWriter& operator<<(std::vector<int> data) {
		for (auto itr = data.begin(); itr != data.end(); itr++) {
			*this << std::to_string(*itr) << std::endl;
		}
		return *this;
	}
	void release() {
		this->close();
	}
};

const float PI = 3.1415926534f;

enum WAVE_TYPE {
	SIN,
	SQUARE,
	SWATOOTH,
	TRIANGLE
};

struct WaveParameter {
	float amplitude;
	int stride;
	int offset;
	int frequence;
	WAVE_TYPE type;

};

class WaveGenerator {
private:
	WaveParameter parameter_;
	std::vector<float> wave_data_;
public:
	
};

class PlexonStimulatorControl {
	ComPtr<ID2D1Factory> d2d_factory_;
	ComPtr<ID2D1HwndRenderTarget> d2d_target_;
	HWND wnd_;
public:
	PlexonStimulatorControl(HWND wnd) :wnd_(wnd) {}
	virtual ~PlexonStimulatorControl() {}
	HRESULT Initialize();
};
HRESULT PlexonStimulatorControl::Initialize() {

	auto res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED, &d2d_factory_);
	if (FAILED(res))return res;

	res = d2d_factory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(wnd_), &d2d_target_);

	return S_OK;
}


LRESULT __stdcall WndProc(HWND wnd, unsigned msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(wnd, msg, wparam, lparam);
}

int __stdcall WinMain(HINSTANCE inst, HINSTANCE prev_inst, char* cmd_line, int cmd_show) {
	int width = 1200, height = 900;
	HWND wnd; {
		std::wstring window_name = L"PlexonStimulatorControl";
		WNDCLASS wc = {};
		wc.hInstance = inst;
		wc.lpfnWndProc = WndProc;
		wc.lpszClassName = window_name.c_str();
		auto res = RegisterClassW(&wc);
		if (!res)return -1;
		wnd = CreateWindowExW(0u, window_name.c_str(), window_name.c_str(), 
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, nullptr, nullptr, inst, nullptr);
		if (!wnd)return -2;
		ShowWindow(wnd, cmd_show);
	}

	std::vector<int> data(10, 100);
	WaveWriter writer("test.txt");
	writer << data;
	writer.release();

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}