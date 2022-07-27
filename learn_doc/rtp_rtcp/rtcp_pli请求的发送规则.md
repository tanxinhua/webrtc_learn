

##  开始没有收到I帧请求发送

**video_rtp_depacketizer_h264.cc**

```C++
  if (original_nal_type == H264::NaluType::kIdr) {
    parsed_payload->video_header.frame_type = VideoFrameType::kVideoFrameKey;
  } else {
    parsed_payload->video_header.frame_type = VideoFrameType::kVideoFrameDelta;
  }
```

**rtp_video_stream_receiver2.cc**

```c++
  // If frames arrive before a key frame, they would not be decodable.
  // In that case, request a key frame ASAP.
  if (!has_received_frame_) {
    if (frame->FrameType() != VideoFrameType::kVideoFrameKey) {
      // |loss_notification_controller_|, if present, would have already
      // requested a key frame when the first packet for the non-key frame
      // had arrived, so no need to replicate the request.
      if (!loss_notification_controller_) { //默认是关闭状态
        RequestKeyFrame();//发送pli
      }
#ifdef JAYA_OPTIMIZATION
    } else {
      has_received_frame_ = true;
    }
#else
    }
    has_received_frame_ = true;
#endif
  }
```

*loss_notification_controller_* 默认是null

VideoFrameType::kVideoFrameKey 表示I帧



##  到I帧但没有收到pps sps vps 请求发送

**h264_sps_pps_tracker.cc**

```c++
          if (nalu.pps_id == -1) {
            RTC_LOG(LS_WARNING) << "No PPS id in IDR nalu.";
            return {kRequestKeyframe};
          }

          pps = pps_data_.find(nalu.pps_id);
          if (pps == pps_data_.end()) {
            RTC_LOG(LS_WARNING)
                << "No PPS with id << " << nalu.pps_id << " received";
            return {kRequestKeyframe};
          }

          sps = sps_data_.find(pps->second.sps_id);
          if (sps == sps_data_.end()) {
            RTC_LOG(LS_WARNING)
                << "No SPS with id << " << pps->second.sps_id << " received";
            return {kRequestKeyframe};
          }
```

**rtp_video_stream_receiver2.cc**

```c++
    switch (fixed.action) {
      case video_coding::H264SpsPpsTracker::kRequestKeyframe:
        rtcp_feedback_buffer_.RequestKeyFrame();//发送pli
        rtcp_feedback_buffer_.SendBufferedRtcpFeedback();
        ABSL_FALLTHROUGH_INTENDED;
      case video_coding::H264SpsPpsTracker::kDrop:
        return;
      case video_coding::H264SpsPpsTracker::kInsert:
        packet->video_payload = std::move(fixed.bitstream);
        break;
    }
```



## 组帧buffer满了还没有组帧成功发送

**packet_buffer.cc**

```c++
    // Packet buffer is still full since we were unable to expand the buffer.
    if (buffer_[index] != nullptr) {
      // Clear the buffer, delete payload, and return false to signal that a
      // new keyframe is needed.
      RTC_LOG(LS_WARNING) << "Clear PacketBuffer and request key frame.";
      ClearInternal();
      result.buffer_cleared = true;
      return result;
    }
```



**rtp_video_stream_receiver2.cc**

```c++
  RTC_DCHECK(frame_boundary);
  if (result.buffer_cleared) {
    RequestKeyFrame();
  }
```