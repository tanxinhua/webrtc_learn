/**
* Copyright 2022 Tencent Inc.  All rights reserved.
* Author:
*/


#include "engine_base/meta_data.h"
#include <assert.h>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include "engine_base/debug_log.h"
#include "engine_base/jaya_object_impl.h"

namespace mediaengine {
namespace enginebase {

class MetaDataImpl : public JayaObjectImpl {
 public:
  explicit MetaDataImpl(const char* uri) : uri_string_(uri) { Reset(); }
  explicit MetaDataImpl(const wchar_t* uri) : uri_string_(uri) { Reset(); }
  explicit MetaDataImpl(const JayaString& uri) : uri_string_(uri) { Reset(); }
  void Reset() {
    video_width_ = 0;
    video_height_ = 0;
    video_lines_ = 0;
    pixel_width_ = 0;
    pixel_height_ = 0;
    for (int i = 0; i < sizeof(video_stride_) / sizeof(video_stride_[0]); i++) {
      video_stride_[i] = 0;
    }
    channels_ = 0;
    sample_rate_ = 0;
    audio_bpp_ = 0;
    block_align_ = 0;
    video_frame_rate_ = 0;
    video_bit_rate_ = 0;
    audio_bit_rate_ = 0;
    context_ = 0;
    fourcc_ = 0;
    send_audio_only_ = false;
    subtitle_ = false;
    video_x_position_ = 0;
    video_y_position_ = 0;
    instance_ = -1;
    audio_volume_ = 0.0f;
  }
  virtual ~MetaDataImpl() {}
  const char* GetUri() { return uri_string_; }
  const wchar_t* GetWUri() { return uri_string_; }
  int SetUri(const char* uri) {
    if (NULL == uri) return -2;
    uri_string_ = JayaString(uri);
    return 0;
  }
  int SetUri(const wchar_t* uri) {
    if (NULL == uri) return -2;
    uri_string_ = JayaString(uri);
    return 0;
  }
  int SetUri(const JayaString& uri) {
    if (!uri) return -2;
    uri_string_ = uri;
    return 0;
  }
  int GetProperty(const char* key, JayaString* p_value) {
    if (key == NULL) return -2;
    assert(p_value);
    std::map<std::string, JayaString>::iterator itr = properties_.find(key);
    if (itr == properties_.end()) return -1;
    *p_value = itr->second;
    return 0;
  }
  int SetProperty(const char* key, const JayaString& value) {
    if (key == NULL) return -2;
    properties_[key] = value;
    return 0;
  }

  int GetMediaSideInfo(std::vector<unsigned char>* p_value) const {
    *p_value = side_data_;
    return 0;
  }

  int SetMediaSideInfo(const std::vector<unsigned char>& value) {
    side_data_ = value;
    return 0;
  }

  int GetServerIps(std::map<std::string, std::string>* p_url_ips_value) const {
    *p_url_ips_value = url_ips_;
    return 0;
  }

  int SetServerIps(const std::map<std::string, std::string>& url_ips_value) {
    url_ips_ = url_ips_value;
    return 0;
  }

  int RemoveProperty(const char* key) {
    if (key == NULL) return -2;
    properties_.erase(key);
    return 0;
  }
  int RemoveProperties() {
    properties_.clear();
    return 0;
  }
  int CopyProperties(MetaDataImpl* meta_data) {
    if (NULL == meta_data) {
      this->properties_.clear();
    } else {
      this->properties_ = meta_data->properties_;
    }
    return 0;
  }
  int UpdateProperties(MetaDataImpl* meta_data) {
    if (NULL == meta_data) {
      return -1;
    }
    for (auto itr = meta_data->properties_.begin(); itr != meta_data->properties_.end(); ++itr) {
      properties_[itr->first] = itr->second;
    }

    return 0;
  }

  int GetChildrenCount() { return static_cast<int>(children_.size()); }
  MetaData GetChild(int n) {
    if (n >= children_.size()) return MetaData();
    return children_[n];
  }
  int AppendChild(MetaDataImpl* meta_data) {
    if (meta_data == this) return -3;
    children_.push_back(meta_data);
    return 0;
  }
  PacketBuffer GetExtraData() const { return extra_data_; }
  void SetExtraData(const PacketBuffer& extra) { extra_data_ = extra; }
  int GetVideoWidth() { return video_width_; }
  int GetVideoHeight() { return video_height_; }
  int GetVideoStride(int plane) { return video_stride_[plane]; }
  int GetVideoPosition(int* p_x, int* p_y) {
    *p_x = video_x_position_;
    *p_y = video_y_position_;
    return 0;
  }
  int SetVideoPosition(int x, int y) {
    video_x_position_ = x;
    video_y_position_ = y;
    return 0;
  }
  void SetVideoWidth(int v) { video_width_ = v; }
  void SetVideoHeight(int v) { video_height_ = v; }
  void SetVideoLines(int v) { video_lines_ = v; }
  void SetVideoStride(int v, int plane) { video_stride_[plane] = v; }
  int GetVideoBitrate() const { return video_bit_rate_; }
  void SetVideoBitrate(int v) { video_bit_rate_ = v; }
  int GetVideoMinBitrate() const { return min_video_bit_rate_; }
  void SetVideoMinBitrate(int v) { min_video_bit_rate_ = v; }
  int GetVideoMaxBitrate() const { return max_video_bit_rate_; }
  void SetVideoMaxBitrate(int v) { max_video_bit_rate_ = v; }
  float GetVideoFrameRate() const { return video_frame_rate_; }
  void SetVideoFrameRate(float v) { video_frame_rate_ = v; }
  void SetPixelWidth(int v) { pixel_width_ = v; }
  void SetPixelHeight(int v) { pixel_height_ = v; }
  int GetPixelWidth() const { return pixel_width_; }
  int GetPixelHeight() const { return pixel_height_; }
  int GetVideoLines() const { return video_lines_; }

  void SetAudioChanel(int v) { channels_ = v; }
  int GetAudioChanel() const { return channels_; }
  void SetAudioSampleRate(int v) { sample_rate_ = v; }
  int GetAudioSampleRate() const { return sample_rate_; }
  void SetAudioBitRate(int v) { audio_bit_rate_ = v; }
  int GetAudioBitRate() const { return audio_bit_rate_; }
  void SetAudioBpp(int v) { audio_bpp_ = v; }
  int GetAudioBpp() const { return audio_bpp_; }
  void SetAudioBlockAlign(int v) { block_align_ = v; }
  int GetAudioBlockAlign() const { return block_align_; }
  void SetSendAudioOnly(bool v) { send_audio_only_ = v; }
  bool GetSendAudioOnly() const { return send_audio_only_; }
  void SetAudioVolume(float v) { audio_volume_ = v; }
  float GetAudioVolume() { return audio_volume_; }
  void set_context(void* v) { context_ = v; }
  void* context() const { return context_; }

  bool IsSubtitle() { return subtitle_; }
  void SetSubtitle(bool v) { subtitle_ = v; }

  MetaData Clone();
  void SetFourcc(unsigned int fcc) { fourcc_ = fcc; }
  unsigned int GetFourcc() const { return fourcc_; }
  void SetInstance(unsigned int ist) { instance_ = ist; }
  unsigned int GetInstance() const { return instance_; }
  void DumpProperties();
  void ParseProperties(const char* spliter);
  int GetProperties(JayaArray<JayaString>* p_names, JayaArray<JayaString>* p_values) {
    auto names = JayaArray<JayaString>::CreateNew(0);
    auto values = JayaArray<JayaString>::CreateNew(0);
    for (std::map<std::string, JayaString>::iterator itr = properties_.begin(); itr != properties_.end(); itr++) {
      names.push(JayaString(itr->first.c_str()));
      values.push(itr->second);
    }
    *p_names = names;
    *p_values = values;
    return static_cast<int>(properties_.size());
  }
  void eval(const std::string& name, const JayaString& value);

 protected:
  JayaString uri_string_;
  std::map<std::string, JayaString> properties_;
  std::vector<unsigned char> side_data_;
  std::map<std::string, std::string> url_ips_;
  std::vector<MetaData> children_;
  PacketBuffer extra_data_;
  int video_width_;
  int video_height_;
  int video_stride_[4];
  int video_lines_;
  int pixel_width_;
  int pixel_height_;
  int video_x_position_;
  int video_y_position_;

  int channels_;
  int sample_rate_;
  int audio_bit_rate_;
  int audio_bpp_;
  int block_align_;
  float audio_volume_;
  bool send_audio_only_;
  bool subtitle_;
  float video_frame_rate_;
  int video_bit_rate_;
  int min_video_bit_rate_;
  int max_video_bit_rate_;
  unsigned int fourcc_;
  unsigned int instance_;
  void* context_;
};

MetaData MetaDataImpl::Clone() {
  MetaDataImpl* newObj = new MetaDataImpl(uri_string_.Clone());
  for (auto p : properties_) {
    newObj->properties_.insert(std::pair<std::string, JayaString>(p.first, p.second.Clone()));
  }

  newObj->side_data_ = side_data_;
  newObj->url_ips_ = url_ips_;

  for (size_t i = 0; i < children_.size(); i++) {
    newObj->children_.push_back(children_[i].Clone());
  }
  newObj->extra_data_ = extra_data_.Clone();
  newObj->video_width_ = video_width_;
  newObj->video_height_ = video_height_;
  for (size_t i = 0; i < sizeof(video_stride_) / sizeof(video_stride_[0]); i++) {
    newObj->video_stride_[i] = video_stride_[i];
  }
  newObj->video_lines_ = video_lines_;
  newObj->pixel_width_ = pixel_width_;
  newObj->pixel_height_ = pixel_height_;
  newObj->video_x_position_ = video_x_position_;
  newObj->video_y_position_ = video_y_position_;
  newObj->channels_ = channels_;
  newObj->sample_rate_ = sample_rate_;
  newObj->audio_bit_rate_ = audio_bit_rate_;
  newObj->audio_bpp_ = audio_bpp_;
  newObj->block_align_ = block_align_;
  newObj->video_frame_rate_ = video_frame_rate_;
  newObj->video_bit_rate_ = video_bit_rate_;
  newObj->min_video_bit_rate_ = min_video_bit_rate_;
  newObj->max_video_bit_rate_ = max_video_bit_rate_;
  newObj->send_audio_only_ = send_audio_only_;
  newObj->audio_volume_ = audio_volume_;
  newObj->fourcc_ = fourcc_;
  newObj->context_ = context_;
  newObj->subtitle_ = subtitle_;
  newObj->instance_ = instance_;
  return MetaData(newObj, false);
}

void MetaDataImpl::DumpProperties() {
  JayaLogI("MetaDataImpl: DumpProperties %s\n", (const char*)this->uri_string_);
  for (std::map<std::string, JayaString>::iterator itr = properties_.begin(); itr != properties_.end(); itr++) {
    JayaLogI("%s=%s\n", itr->first.c_str(), (const char*)itr->second);
  }
}

JAYA_DEFINE_OBJECT(MetaData);

MetaData::MetaData(const char* uri) : pImpl_(new MetaDataImpl(uri)) {}

MetaData::MetaData(const wchar_t* uri) : pImpl_(new MetaDataImpl(uri)) {}

MetaData::MetaData(const JayaString& uri) : pImpl_(new MetaDataImpl(uri)) {}

const char* MetaData::GetUri() const {
  if (!pImpl_) return "";
  return pImpl_->GetUri();
}

const wchar_t* MetaData::GetWUri() const {
  if (!pImpl_) return L"";
  return pImpl_->GetWUri();
}

int MetaData::SetUri(const char* uri) {
  if (!pImpl_) return -1;
  return pImpl_->SetUri(uri);
}

int MetaData::SetUri(const wchar_t* uri) {
  if (!pImpl_) return -1;
  return pImpl_->SetUri(uri);
}

int MetaData::SetUri(const JayaString& uri) {
  if (!pImpl_) return -1;
  return pImpl_->SetUri(uri);
}

int MetaData::GetProperty(const char* key, JayaString* p_value) const {
  if (!pImpl_) return -1;
  return pImpl_->GetProperty(key, p_value);
}

int MetaData::SetProperty(const char* key, const JayaString& value) {
  if (!pImpl_) return -1;
  return pImpl_->SetProperty(key, value);
}

int MetaData::SetProperty(const char* key, const char* value) {
  if (!pImpl_) return -1;
  return pImpl_->SetProperty(key, JayaString(value));
}

int MetaData::SetProperty(const char* key, const wchar_t* value) {
  if (!pImpl_) return -1;
  return pImpl_->SetProperty(key, JayaString(value));
}

int MetaData::RemoveProperty(const char* key) {
  if (!pImpl_) return -1;
  return pImpl_->RemoveProperty(key);
}

int MetaData::RemoveProperties() {
  if (!pImpl_) return -1;
  return pImpl_->RemoveProperties();
}

int MetaData::CopyProperties(const MetaData& source) {
  if (!pImpl_) return -1;
  return pImpl_->CopyProperties(source.pImpl_);
}

int MetaData::UpdateProperties(const MetaData& source) {
  if (!pImpl_) return -1;
  return pImpl_->UpdateProperties(source.pImpl_);
}

int MetaData::GetChildrenCount() const {
  if (!pImpl_) return -1;
  return pImpl_->GetChildrenCount();
}

MetaData MetaData::GetChild(int n) const {
  if (!pImpl_) return MetaData();
  return pImpl_->GetChild(n);
}

int MetaData::AppendChild(const MetaData& meta_data) {
  if (!pImpl_) return -1;
  if (!meta_data) return -2;
  return pImpl_->AppendChild(meta_data.pImpl_);
}

PacketBuffer MetaData::GetExtraData() const {
  if (pImpl_) return pImpl_->GetExtraData();
  return PacketBuffer();
}

void MetaData::SetExtraData(const PacketBuffer& extra) {
  if (pImpl_) pImpl_->SetExtraData(extra);
}

int MetaData::GetVideoWidth() const {
  if (pImpl_) return pImpl_->GetVideoWidth();
  return -1;
}

int MetaData::GetVideoHeight() const {
  if (pImpl_) return pImpl_->GetVideoHeight();
  return -1;
}

int MetaData::GetVideoStride(int plane) const {
  if (pImpl_) return pImpl_->GetVideoStride(plane);
  return -1;
}

void MetaData::SetVideoWidth(int v) {
  if (pImpl_) pImpl_->SetVideoWidth(v);
}

void MetaData::SetVideoHeight(int v) {
  if (pImpl_) pImpl_->SetVideoHeight(v);
}

void MetaData::SetVideoStride(int v, int plane) {
  if (pImpl_) pImpl_->SetVideoStride(v, plane);
}
void MetaData::SetVideoBitrate(int v) {
  if (pImpl_) pImpl_->SetVideoBitrate(v);
}
int MetaData::GetVideoBitrate() const {
  if (pImpl_) return pImpl_->GetVideoBitrate();
  return -1;
}

void MetaData::SetVideoMinBitrate(int v) {
  if (pImpl_) pImpl_->SetVideoMinBitrate(v);
}
int MetaData::GetVideoMinBitrate() const {
  if (pImpl_) return pImpl_->GetVideoMinBitrate();
  return -1;
}

void MetaData::SetVideoMaxBitrate(int v) {
  if (pImpl_) pImpl_->SetVideoMaxBitrate(v);
}
int MetaData::GetVideoMaxBitrate() const {
  if (pImpl_) return pImpl_->GetVideoMaxBitrate();
  return -1;
}


void MetaData::SetVideoFrameRate(float v) {
  if (pImpl_) pImpl_->SetVideoFrameRate(v);
}
float MetaData::GetVideoFrameRate() const {
  if (pImpl_) return pImpl_->GetVideoFrameRate();
  return -1;
}

void MetaData::SetPixelWidth(int v) {
  if (pImpl_) pImpl_->SetPixelWidth(v);
}

void MetaData::SetPixelHeight(int v) {
  if (pImpl_) pImpl_->SetPixelHeight(v);
}

int MetaData::GetPixelWidth() const {
  if (pImpl_) return pImpl_->GetPixelWidth();
  return -1;
}

int MetaData::GetPixelHeight() const {
  if (pImpl_) return pImpl_->GetPixelHeight();
  return -1;
}

void MetaData::SetAudioChanel(int v) {
  if (pImpl_) pImpl_->SetAudioChanel(v);
}
int MetaData::GetAudioChanel() const {
  if (pImpl_) return pImpl_->GetAudioChanel();
  return -1;
}
void MetaData::SetAudioSampleRate(int v) {
  if (pImpl_) pImpl_->SetAudioSampleRate(v);
}
int MetaData::GetAudioSampleRate() const {
  if (pImpl_) return pImpl_->GetAudioSampleRate();
  return -1;
}
int MetaData::GetAudioBitRate() const {
  if (pImpl_) return pImpl_->GetAudioBitRate();
  return -1;
}

void MetaData::SetAudioBitRate(int v) {
  if (pImpl_) pImpl_->SetAudioBitRate(v);
}

void MetaData::SetAudioBpp(int v) {
  if (pImpl_) pImpl_->SetAudioBpp(v);
}
int MetaData::GetAudioBpp() const {
  if (pImpl_) return pImpl_->GetAudioBpp();
  return -1;
}

void MetaData::SetVideoLines(int v) {
  if (pImpl_) pImpl_->SetVideoLines(v);
}

int MetaData::GetVideoLines() const {
  if (pImpl_) return pImpl_->GetVideoLines();
  return -1;
}

void MetaData::SetAudioBlockAlign(int v) {
  if (pImpl_) pImpl_->SetAudioBlockAlign(v);
}
int MetaData::GetAudioBlockAlign() const {
  if (pImpl_) return pImpl_->GetAudioBlockAlign();
  return -1;
}

void MetaData::SetSendAudioOnly(bool v) {
  if (pImpl_) pImpl_->SetSendAudioOnly(v);
}

bool MetaData::GetSendAudioOnly() const {
  if (pImpl_) return pImpl_->GetSendAudioOnly();
  return false;
}
void MetaData::SetAudioVolume(float v) {
  if (pImpl_) pImpl_->SetAudioVolume(v);
}
float MetaData::GetAudioVolume() const {
  if (pImpl_) return pImpl_->GetAudioVolume();
  return false;
}
void MetaData::set_context(void* v) {
  if (pImpl_) pImpl_->set_context(v);
}
void* MetaData::context() const {
  if (pImpl_) return pImpl_->context();
  return 0;
}

MetaData MetaData::Clone() const {
  if (pImpl_) return pImpl_->Clone();
  return MetaData();
}

void MetaData::SetFourcc(unsigned int fcc) {
  if (pImpl_) pImpl_->SetFourcc(fcc);
}

unsigned int MetaData::GetFourcc() const {
  if (pImpl_) return pImpl_->GetFourcc();
  return 0;
}

void MetaData::SetInstance(unsigned int ist) {
  if (pImpl_) pImpl_->SetInstance(ist);
}

unsigned int MetaData::GetInstance() const {
  if (pImpl_) return pImpl_->GetInstance();
  return 0;
}

void MetaData::DumpProperties() const {
  if (pImpl_) pImpl_->DumpProperties();
}

int MetaData::GetVideoPosition(int* p_x, int* p_y) const {
  if (pImpl_) return pImpl_->GetVideoPosition(p_x, p_y);
  return -1;
}

int MetaData::SetVideoPosition(int x, int y) {
  if (pImpl_) return pImpl_->SetVideoPosition(x, y);
  return -1;
}

int MetaData::GetProperty(const char* key, int* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atol(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, int value) { return SetProperty(key, JayaString::asprintf("%d", value)); }

int MetaData::GetProperty(const char* key, uint32_t* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoll(strVal);
    return 0;
  }
  return -1;
}

int MetaData::SetProperty(const char* key, uint32_t value) {
  return SetProperty(key, JayaString::asprintf("%lu", value));
}

int MetaData::GetProperty(const char* key, uint16_t* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoi(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, uint16_t value) {
  return SetProperty(key, JayaString::asprintf("%u", value));
}
int MetaData::GetProperty(const char* key, int16_t* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoi(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, int16_t value) {
    return SetProperty(key, JayaString::asprintf("%d", value));
}
int MetaData::GetProperty(const char* key, int64_t* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoll(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, int64_t value) {
  return SetProperty(key, JayaString::asprintf("%lld", value));
}
int MetaData::GetProperty(const char* key, uint64_t* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = std::stoull(std::string(strVal));
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, uint64_t value) {
  return SetProperty(key, JayaString::asprintf("%llu", value));
}
int MetaData::GetProperty(const char* key, float* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = static_cast<float>(atof(strVal));
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, float value) { return SetProperty(key, JayaString::asprintf("%f", value)); }

int MetaData::GetProperty(const char* key, double* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atof(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, double value) {
  return SetProperty(key, JayaString::asprintf("%llf", value));
}

int MetaData::GetProperty(const char* key, char* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoi(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, char value) { return SetProperty(key, JayaString::asprintf("%d", value)); }
int MetaData::GetProperty(const char* key, unsigned char* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoi(strVal);
    return 0;
  }
  return -1;
}
int MetaData::SetProperty(const char* key, unsigned char value) {
  return SetProperty(key, JayaString::asprintf("%d", value));
}
int MetaData::SetProperty(const char* key, bool value) { return SetProperty(key, JayaString::asprintf("%d", value)); }
int MetaData::GetProperty(const char* key, bool* p_value) const {
  JayaString strVal;
  if (!GetProperty(key, &strVal)) {
    *p_value = atoi(strVal);
    return 0;
  }
  return -1;
}
bool MetaData::IsSubtitle() const {
  if (pImpl_) return pImpl_->IsSubtitle();
  return false;
}
void MetaData::SetSubtitle(bool v) {
  if (pImpl_) pImpl_->SetSubtitle(v);
}

int MetaData::GetMediaSideInfo(std::vector<unsigned char>* p_value) const {
  if (pImpl_) return pImpl_->GetMediaSideInfo(p_value);
  return -1;
}

int MetaData::SetMediaSideInfo(const std::vector<unsigned char>& value) {
  if (pImpl_) return pImpl_->SetMediaSideInfo(value);
  return -1;
}

int MetaData::GetServerIps(std::map<std::string, std::string>* p_url_ips_value) const {
  if (pImpl_) return pImpl_->GetServerIps(p_url_ips_value);
  return -1;
}

int MetaData::SetServerIps(const std::map<std::string, std::string>& url_ips_value) {
  if (pImpl_) return pImpl_->SetServerIps(url_ips_value);
  return -1;
}

void MetaDataImpl::eval(const std::string& name, const JayaString& value) {
  if (name == "VideoWidth") {
    this->video_width_ = atoi(value);
  } else if (name == "VideoHeight") {
    this->video_height_ = atoi(value);
  } else if (name == "VideoFrameRate") {
    this->video_frame_rate_ = static_cast<float>(atof(value));
  } else if (name == "VideoBitrate") {
    this->video_bit_rate_ = static_cast<int>(atof(value));
  } else if (name == "AudioChanel") {
    this->channels_ = atoi(value);
  } else if (name == "AudioSampleRate") {
    this->sample_rate_ = atoi(value);
  } else if (name == "AudioBpp") {
    this->audio_bpp_ = atoi(value);
  } else if (name == "AudioBitRate") {
    this->audio_bit_rate_ = atoi(value);
  } else if (name == "AudioBlockAlign") {
    this->block_align_ = atoi(value);
  } else {
    properties_[name] = value;
  }
}

static JayaString StripUrl(JayaString value) {
  char* temp = const_cast<char*>((const char*)value);
  char* raw = temp;
  while (*temp) {
    if (*temp++ == '%' && *temp++ != '\0' && *temp++ != '\0') {
      unsigned char bh = temp[-2] - '0';
      unsigned char bl = temp[-1] - '0';
      if (bh > 49)
        bh -= 39;
      else if (bh > 9)
        bh -= 7;
      if (bl > 49)
        bl -= 39;
      else if (bl > 9)
        bl -= 7;
      *raw++ = (bh << 4) | bl;
    } else {
      *raw++ = temp[-1];
    }
  }
  *raw = '\0';
  return value;
}

void MetaDataImpl::ParseProperties(const char* spliter) {
  const char* next_spliter = spliter;
  while (next_spliter != NULL) {
    next_spliter = strchr(spliter, '&');
    const char* eq = strchr(spliter, '=');
    if (eq != NULL) {
      std::string name(spliter, eq - spliter);
      ++eq;
      const char* last = next_spliter ? next_spliter : eq + strlen(eq);
      eval(name, StripUrl(JayaString::fromUtf8(eq, static_cast<int>(last - eq))));
    }
    spliter = next_spliter + 1;
  }
}

MetaData MetaData::Parse(const char* url) {
  const char* query = strchr(url, '?');
  if (query == NULL) {
    return MetaData(StripUrl(JayaString::fromUtf8(url)));
  }
  MetaDataImpl* impl = new MetaDataImpl(
      JayaString::fromUtf8(url, static_cast<int>(query - url)));
  impl->ParseProperties(++query);
  return MetaData(impl, false);
}

int MetaData::GetProperties(JayaArray<JayaString>* p_names, JayaArray<JayaString>* p_values) {
  if (pImpl_) return pImpl_->GetProperties(p_names, p_values);
  return -1;
}

int MetaData::UpdateFromUtf8(const char* urlUtf8) {
  const char* query = strchr(urlUtf8, '?');
  if (query == NULL) return -1;
  pImpl_->ParseProperties(query + 1);
  pImpl_->SetUri(JayaString::fromUtf8(urlUtf8, static_cast<int>(query - urlUtf8)));
  return 0;
}

}  // namespace enginebase
}  // namespace mediaengine
