// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BubbleLibCfg.proto

#ifndef PROTOBUF_BubbleLibCfg_2eproto__INCLUDED
#define PROTOBUF_BubbleLibCfg_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_BubbleLibCfg_2eproto();
void protobuf_AssignDesc_BubbleLibCfg_2eproto();
void protobuf_ShutdownFile_BubbleLibCfg_2eproto();

class BubbleLibCfg;
class BubbleLibCfgSet;

// ===================================================================

class BubbleLibCfg : public ::google::protobuf::MessageLite {
 public:
  BubbleLibCfg();
  virtual ~BubbleLibCfg();

  BubbleLibCfg(const BubbleLibCfg& from);

  inline BubbleLibCfg& operator=(const BubbleLibCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const BubbleLibCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const BubbleLibCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(BubbleLibCfg* other);

  // implements Message ----------------------------------------------

  BubbleLibCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BubbleLibCfg& from);
  void MergeFrom(const BubbleLibCfg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 ID = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIDFieldNumber = 1;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);

  // required uint32 CDTime = 2;
  inline bool has_cdtime() const;
  inline void clear_cdtime();
  static const int kCDTimeFieldNumber = 2;
  inline ::google::protobuf::uint32 cdtime() const;
  inline void set_cdtime(::google::protobuf::uint32 value);

  // required uint32 Prob = 3;
  inline bool has_prob() const;
  inline void clear_prob();
  static const int kProbFieldNumber = 3;
  inline ::google::protobuf::uint32 prob() const;
  inline void set_prob(::google::protobuf::uint32 value);

  // repeated string Words = 4;
  inline int words_size() const;
  inline void clear_words();
  static const int kWordsFieldNumber = 4;
  inline const ::std::string& words(int index) const;
  inline ::std::string* mutable_words(int index);
  inline void set_words(int index, const ::std::string& value);
  inline void set_words(int index, const char* value);
  inline void set_words(int index, const char* value, size_t size);
  inline ::std::string* add_words();
  inline void add_words(const ::std::string& value);
  inline void add_words(const char* value);
  inline void add_words(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& words() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_words();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.BubbleLibCfg)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_cdtime();
  inline void clear_has_cdtime();
  inline void set_has_prob();
  inline void clear_has_prob();

  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 cdtime_;
  ::google::protobuf::RepeatedPtrField< ::std::string> words_;
  ::google::protobuf::uint32 prob_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_BubbleLibCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_BubbleLibCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_BubbleLibCfg_2eproto();
  friend void protobuf_ShutdownFile_BubbleLibCfg_2eproto();

  void InitAsDefaultInstance();
  static BubbleLibCfg* default_instance_;
};
// -------------------------------------------------------------------

class BubbleLibCfgSet : public ::google::protobuf::MessageLite {
 public:
  BubbleLibCfgSet();
  virtual ~BubbleLibCfgSet();

  BubbleLibCfgSet(const BubbleLibCfgSet& from);

  inline BubbleLibCfgSet& operator=(const BubbleLibCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const BubbleLibCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const BubbleLibCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(BubbleLibCfgSet* other);

  // implements Message ----------------------------------------------

  BubbleLibCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BubbleLibCfgSet& from);
  void MergeFrom(const BubbleLibCfgSet& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .com.cfg.vo.BubbleLibCfg bubbleLibCfg = 1;
  inline int bubblelibcfg_size() const;
  inline void clear_bubblelibcfg();
  static const int kBubbleLibCfgFieldNumber = 1;
  inline const ::com::cfg::vo::BubbleLibCfg& bubblelibcfg(int index) const;
  inline ::com::cfg::vo::BubbleLibCfg* mutable_bubblelibcfg(int index);
  inline ::com::cfg::vo::BubbleLibCfg* add_bubblelibcfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::BubbleLibCfg >&
      bubblelibcfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::BubbleLibCfg >*
      mutable_bubblelibcfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.BubbleLibCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::BubbleLibCfg > bubblelibcfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_BubbleLibCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_BubbleLibCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_BubbleLibCfg_2eproto();
  friend void protobuf_ShutdownFile_BubbleLibCfg_2eproto();

  void InitAsDefaultInstance();
  static BubbleLibCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// BubbleLibCfg

// required uint32 ID = 1;
inline bool BubbleLibCfg::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BubbleLibCfg::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BubbleLibCfg::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BubbleLibCfg::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 BubbleLibCfg::id() const {
  return id_;
}
inline void BubbleLibCfg::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required uint32 CDTime = 2;
inline bool BubbleLibCfg::has_cdtime() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void BubbleLibCfg::set_has_cdtime() {
  _has_bits_[0] |= 0x00000002u;
}
inline void BubbleLibCfg::clear_has_cdtime() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void BubbleLibCfg::clear_cdtime() {
  cdtime_ = 0u;
  clear_has_cdtime();
}
inline ::google::protobuf::uint32 BubbleLibCfg::cdtime() const {
  return cdtime_;
}
inline void BubbleLibCfg::set_cdtime(::google::protobuf::uint32 value) {
  set_has_cdtime();
  cdtime_ = value;
}

// required uint32 Prob = 3;
inline bool BubbleLibCfg::has_prob() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void BubbleLibCfg::set_has_prob() {
  _has_bits_[0] |= 0x00000004u;
}
inline void BubbleLibCfg::clear_has_prob() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void BubbleLibCfg::clear_prob() {
  prob_ = 0u;
  clear_has_prob();
}
inline ::google::protobuf::uint32 BubbleLibCfg::prob() const {
  return prob_;
}
inline void BubbleLibCfg::set_prob(::google::protobuf::uint32 value) {
  set_has_prob();
  prob_ = value;
}

// repeated string Words = 4;
inline int BubbleLibCfg::words_size() const {
  return words_.size();
}
inline void BubbleLibCfg::clear_words() {
  words_.Clear();
}
inline const ::std::string& BubbleLibCfg::words(int index) const {
  return words_.Get(index);
}
inline ::std::string* BubbleLibCfg::mutable_words(int index) {
  return words_.Mutable(index);
}
inline void BubbleLibCfg::set_words(int index, const ::std::string& value) {
  words_.Mutable(index)->assign(value);
}
inline void BubbleLibCfg::set_words(int index, const char* value) {
  words_.Mutable(index)->assign(value);
}
inline void BubbleLibCfg::set_words(int index, const char* value, size_t size) {
  words_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BubbleLibCfg::add_words() {
  return words_.Add();
}
inline void BubbleLibCfg::add_words(const ::std::string& value) {
  words_.Add()->assign(value);
}
inline void BubbleLibCfg::add_words(const char* value) {
  words_.Add()->assign(value);
}
inline void BubbleLibCfg::add_words(const char* value, size_t size) {
  words_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
BubbleLibCfg::words() const {
  return words_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
BubbleLibCfg::mutable_words() {
  return &words_;
}

// -------------------------------------------------------------------

// BubbleLibCfgSet

// repeated .com.cfg.vo.BubbleLibCfg bubbleLibCfg = 1;
inline int BubbleLibCfgSet::bubblelibcfg_size() const {
  return bubblelibcfg_.size();
}
inline void BubbleLibCfgSet::clear_bubblelibcfg() {
  bubblelibcfg_.Clear();
}
inline const ::com::cfg::vo::BubbleLibCfg& BubbleLibCfgSet::bubblelibcfg(int index) const {
  return bubblelibcfg_.Get(index);
}
inline ::com::cfg::vo::BubbleLibCfg* BubbleLibCfgSet::mutable_bubblelibcfg(int index) {
  return bubblelibcfg_.Mutable(index);
}
inline ::com::cfg::vo::BubbleLibCfg* BubbleLibCfgSet::add_bubblelibcfg() {
  return bubblelibcfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::BubbleLibCfg >&
BubbleLibCfgSet::bubblelibcfg() const {
  return bubblelibcfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::BubbleLibCfg >*
BubbleLibCfgSet::mutable_bubblelibcfg() {
  return &bubblelibcfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_BubbleLibCfg_2eproto__INCLUDED