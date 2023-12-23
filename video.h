#pragma once
#include <map>

#include "graphical.h"
#include "time.h"
#include "lib/video_reader.hpp"

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
    std::map<double, uint8_t*> frame_buffer_;
    std::string filename_;
    bool loop_ = false;
    int64_t end_pts = 0;

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
    }

    ~Video()
    {
        free(frame_data_);
        video_reader_close(&vr_state_);
        texture_->Release();
    }

    void Update()
    {
        time_ += Time::GetDeltaTime();

        //load frame into buffer
        int64_t pts;
        if (!video_reader_read_frame(&vr_state_, frame_data_, &pts))
        {
            std::cout << "Couldn't load video frame\n";
            return;
        }
        if(pts == end_pts && loop_)
        {
            time_ = 0.0;
            video_reader_seek_frame(&vr_state_, 0);
        }
        if (end_pts < pts)
        {
            end_pts = pts;
        }

        double pt_in_seconds = pts * (double)vr_state_.time_base.num / (double)vr_state_.time_base.den;

        frame_buffer_.insert(std::make_pair(pt_in_seconds, frame_data_));

        //Get frame from buffer
        if (frame_buffer_.size() > 0)
        {
            double pts = frame_buffer_.begin()->first;

            if (time_ < pts)
            {
                return;
            }

            frame_data_ = frame_buffer_.begin()->second;
            frame_buffer_.erase(frame_buffer_.begin());

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
        }
    }

    void Draw()
    {
        // Draw the texture with ImGui
        ImGui::Begin(filename_.c_str());
        ImGui::Image((void*)texture_view_, ImVec2(vr_state_.width * scale_, vr_state_.height * scale_));
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
};
