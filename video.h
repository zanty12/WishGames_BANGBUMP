#pragma once
#include <unordered_map>
#include <thread>

#include "graphical.h"
#include "time.h"
#include "lib/video_reader.hpp"

struct frame_data
{
    int64_t ts;
    uint8_t* data;
};

class Video
{
private:
    VideoReaderState vr_state_;
    ID3D11Texture2D* texture_;
    ID3D11ShaderResourceView* texture_view_;
    uint8_t* frame_data_;
    bool first_frame_ = true;
    double time_ = 0.0;
    float scale_ = 1.0f;
    std::list<frame_data> frame_buffer_;
    std::string filename_;
    bool loop_ = false;
    int64_t end_pts_ = 0;
    double pts_ = 0;
    bool loaded_ = false;
    std::list<frame_data>::iterator frame_it_;
    Vector2 size_;
    Vector2 window_pos_ = Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2);

public:
    Video(const char* filename)
    {
        filename_ = filename;
        if (!video_reader_open(&vr_state_, filename))
        {
            std::cout << "Couldn't open video file (make sure you set a video file that exists)\n";
        }

        // Create a DirectX11 texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = vr_state_.width;
        desc.Height = vr_state_.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        Graphical::GetDevice().Get()->CreateTexture2D(&desc, NULL, &texture_);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        Graphical::GetDevice().Get()->CreateShaderResourceView(texture_, &srvDesc, &texture_view_);

        // Allocate frame buffer
        constexpr int ALIGNMENT = 128;
        const int frame_width = vr_state_.width;
        const int frame_height = vr_state_.height;
        frame_data_ = (uint8_t*)_aligned_malloc(frame_width * frame_height * 4, ALIGNMENT);
        if (frame_data_ == 0)
        {
            std::cout << "Couldn't allocate frame buffer\n";
        }

        size_ = Vector2(frame_width, frame_height);
    }

    ~Video()
    {
        _aligned_free(frame_data_);
        for (auto frame : frame_buffer_)
        {
            _aligned_free(frame.data);
        }
        video_reader_close(&vr_state_);
        texture_->Release();
    }

    void Update()
    {
        time_ += Time::GetDeltaTime();

        //load frame data
        int64_t pts = 0;
        if (!loaded_)
        {
            if (!video_reader_read_frame(&vr_state_, frame_data_, &pts))
            {
                std::cout << "Couldn't load video frame\n";
                return;
            }
            //buffer frame
            frame_data frame;
            frame.ts = pts;
            frame.data = (uint8_t*)_aligned_malloc(vr_state_.width * vr_state_.height * 4, 128);
            memcpy(frame.data, frame_data_, vr_state_.width * vr_state_.height * 4);
            frame_buffer_.push_back(frame);
            if (first_frame_)
            {
                frame_it_ = frame_buffer_.begin();
                first_frame_ = false;
            }
        }
        if (pts == end_pts_ && !loaded_ && end_pts_ != 0)
        {
            loaded_ = true;
        }
        //loop back if at end of video
        if (loop_ && frame_it_->ts == end_pts_ && end_pts_ != 0)
        {
            frame_it_ = frame_buffer_.begin();
            time_ = 0.0;
        }

        //count max pts
        if (pts > end_pts_)
        {
            end_pts_ = pts;
        }

        //load next frame
        if (frame_buffer_.size() > 0 && time_ > frame_it_->ts * (double)vr_state_.time_base.num / (double)vr_state_.
            time_base.den)
        {
            frame_data frame = *frame_it_;
            memcpy(frame_data_, frame.data, vr_state_.width * vr_state_.height * 4);
            pts_ = frame.ts;
            //load frame data to texture
            D3D11_MAPPED_SUBRESOURCE resource;
            Graphical::GetDevice().GetContext()->Map(texture_, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
            uint8_t* dest = (uint8_t*)resource.pData;
            for (int y = 0; y < vr_state_.height; ++y)
            {
                memcpy(dest, frame_data_ + y * vr_state_.width * 4, vr_state_.width * 4);
                dest += resource.RowPitch;
            }
            Graphical::GetDevice().GetContext()->Unmap(texture_, 0);

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            srvDesc.Texture2D.MostDetailedMip = 0;
            Graphical::GetDevice().Get()->CreateShaderResourceView(texture_, &srvDesc, &texture_view_);
            if (frame_it_ != --frame_buffer_.end())
                ++frame_it_;
        }
    }

    void Draw()
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        bool open = true;
        ImGui::SetNextWindowPos(ImVec2(window_pos_.x - size_.x * scale_ / 2, window_pos_.y - size_.y * scale_ / 2));
        // Draw the texture with ImGui
        ImGui::Begin(filename_.c_str(), &open, window_flags);
        //ImGui::Text("time: %.2f, pts: %.2f", time_, pts_);
        ImGui::Image((void*)texture_view_, ImVec2(size_.x * scale_, size_.y * scale_));
        ImGui::End();
    }

    void SetScale(float scale)
    {
        scale_ = scale;
    }

    void SetLoop(bool loop)
    {
        loop_ = loop;
    }

    void SetSize(Vector2 size)
    {
        size_ = size;
    }

    void SetWindowPos(Vector2 pos)
    {
        window_pos_ = pos;
    }

};
