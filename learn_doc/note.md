

### Rtcp

#### rtcp_sender.cc

```c++
method_ 

enum class RtcpMode { kOff, kCompound, kReducedSize };
```

RtcpMode  koff可以关闭rtcp

#### rtp_rtcp_config.h

```C++
// Configuration file for RTP utilities (RTPSender, RTPReceiver ...)
namespace webrtc {
enum { kDefaultMaxReorderingThreshold = 50 };  // In sequence numbers.
enum { kRtcpMaxNackFields = 253 };

enum { RTCP_SEND_BEFORE_KEY_FRAME_MS = 100 };
enum { RTCP_MAX_REPORT_BLOCKS = 31 };  // RFC 3550 page 37
}  // namespace webrtc
```

#### on_keyframe_ms_list_ 关键帧请求怎么处理

#### video_codec_interface.h

h264分包选择 stap fu-a类型选择

```C++
// Packetization modes are defined in RFC 6184 section 6
// Due to the structure containing this being initialized with zeroes
// in some places, and mode 1 being default, mode 1 needs to have the value
// zero. https://crbug.com/webrtc/6803
enum class H264PacketizationMode {
  NonInterleaved = 0,  // Mode 1 - STAP-A, FU-A is allowed
  SingleNalUnit        // Mode 0 - only single NALU allowed
};
```

