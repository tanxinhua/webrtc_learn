/**
* Copyright 2022 Tencent Inc.  All rights reserved.
* Author:
*/
#pragma once
#include <map>
#include <vector>
#include <string>
#include "engine_base/jaya_array.h"
#include "engine_base/jaya_string.h"
#include "engine_base/packet_buffer.h"

#define META_COMPARE_FOURCC(x, y) (x[0] == y[0] && x[1] == y[1] && x[2] == y[2] && x[3] == y[3])

namespace mediaengine {
namespace enginebase {

class MetaDataImpl;
class MetaData {
 public:
  JAYA_DECLARE_OBJECT(MetaData);
  explicit MetaData(const char* uri);
  explicit MetaData(const JayaString& uri);
  explicit MetaData(const wchar_t* uri);
  const char* GetUri() const;
  const wchar_t* GetWUri() const;
  int SetUri(const char* uri);
  int SetUri(const wchar_t* uri);
  int SetUri(const JayaString& uri);
  int GetProperty(const char* key, JayaString* value) const;
  int SetProperty(const char* key, const JayaString& value);
  int SetProperty(const char* key, const char* value);
  int SetProperty(const char* key, const wchar_t* value);
  int GetProperty(const char* key, int* value) const;
  int SetProperty(const char* key, int value);
  int GetProperty(const char* key, uint32_t* p_value) const;
  int SetProperty(const char* key, uint32_t value);
  int GetProperty(const char* key, int16_t* p_value) const;
  int SetProperty(const char* key, int16_t value);
  int GetProperty(const char* key, uint16_t* p_value) const;
  int SetProperty(const char* key, uint16_t value);
  int GetProperty(const char* key, int64_t* p_value) const;
  int SetProperty(const char* key, int64_t value);
  int GetProperty(const char* key, uint64_t* p_value) const;
  int SetProperty(const char* key, uint64_t value);
  int GetProperty(const char* key, float* p_value) const;
  int SetProperty(const char* key, float value);
  int GetProperty(const char* key, double* p_value) const;
  int SetProperty(const char* key, double value);
  int GetProperty(const char* key, char* p_value) const;
  int SetProperty(const char* key, char value);
  int GetProperty(const char* key, unsigned char* p_value) const;
  int SetProperty(const char* key, unsigned char value);
  int GetProperty(const char* key, bool* p_value) const;
  int SetProperty(const char* key, bool value);
  template <class T>
  int GetStructProperty(const char* key, T* p_out) const {
    JayaString str;
    if (GetProperty(key, &str) < 0) return -1;
    return str.copyBytes(reinterpret_cast<unsigned char*>(p_out), sizeof(T));
  }

  template <class T>
  int SetStructProperty(const char* key, const T& in) {
    JayaString str((const char*)&in, sizeof(in));
    return SetProperty(key, str);
  }

  int RemoveProperty(const char* key);
  int RemoveProperties();
  int CopyProperties(const MetaData& source);
  int UpdateProperties(const MetaData& source);
  int GetChildrenCount() const;
  MetaData GetChild(int n) const;
  int AppendChild(const MetaData& meta_data);

  PacketBuffer GetExtraData() const;
  void SetExtraData(const PacketBuffer& extra);
  int GetVideoWidth() const;
  int GetVideoHeight() const;
  int GetVideoPosition(int* p_x, int* p_y) const;
  int GetVideoStride(int plane = 0) const;
  void SetVideoWidth(int v);
  void SetVideoHeight(int v);
  int SetVideoPosition(int x, int y);
  void SetVideoStride(int v, int plane = 0);
  float GetVideoFrameRate() const;
  void SetVideoFrameRate(float v);
  int GetVideoBitrate() const;
  void SetVideoBitrate(int v);
  int GetVideoMinBitrate() const;
  void SetVideoMinBitrate(int v);
  int GetVideoMaxBitrate() const;
  void SetVideoMaxBitrate(int v);
  void SetPixelWidth(int v);
  void SetPixelHeight(int v);
  int GetPixelWidth() const;
  int GetPixelHeight() const;
  void SetVideoLines(int v);
  int GetVideoLines() const;

  void SetAudioChanel(int v);
  int GetAudioChanel() const;
  void SetAudioSampleRate(int v);
  int GetAudioSampleRate() const;
  void SetAudioBitRate(int v);
  int GetAudioBitRate() const;
  void SetAudioBpp(int v);
  int GetAudioBpp() const;
  void SetAudioBlockAlign(int v);
  int GetAudioBlockAlign() const;
  void SetSendAudioOnly(bool v);
  bool GetSendAudioOnly() const;
  void SetAudioVolume(float v);
  float GetAudioVolume() const;
  void set_context(void* v);
  void* context() const;
  int GetMediaSideInfo(std::vector<unsigned char>* p_value) const;
  int SetMediaSideInfo(const std::vector<unsigned char>& value);
  int GetServerIps(std::map<std::string, std::string>* p_url_ips_value) const;
  int SetServerIps(const std::map<std::string, std::string>& url_ips_value);

  bool IsSubtitle() const;
  void SetSubtitle(bool v);

  MetaData Clone() const;
  void SetFourcc(unsigned int fcc);
  unsigned int GetFourcc() const;
  void SetInstance(unsigned int ist);
  unsigned int GetInstance() const;
  void DumpProperties() const;
  static MetaData Parse(const char* urlUtf8);
  int GetProperties(JayaArray<JayaString>* p_names, JayaArray<JayaString>* p_values);
  int UpdateFromUtf8(const char* urlUtf8);

 protected:
  MetaDataImpl* pImpl_;
};

}  // namespace enginebase
}  // namespace mediaengine
