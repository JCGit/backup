// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: EquipLevelCfg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "EquipLevelCfg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

void protobuf_ShutdownFile_EquipLevelCfg_2eproto() {
  delete EquipLevelCfg::default_instance_;
  delete EquipLevelCfgSet::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_EquipLevelCfg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_EquipLevelCfg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  EquipLevelCfg::default_instance_ = new EquipLevelCfg();
  EquipLevelCfgSet::default_instance_ = new EquipLevelCfgSet();
  EquipLevelCfg::default_instance_->InitAsDefaultInstance();
  EquipLevelCfgSet::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_EquipLevelCfg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_EquipLevelCfg_2eproto_once_);
void protobuf_AddDesc_EquipLevelCfg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_EquipLevelCfg_2eproto_once_,
                 &protobuf_AddDesc_EquipLevelCfg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_EquipLevelCfg_2eproto {
  StaticDescriptorInitializer_EquipLevelCfg_2eproto() {
    protobuf_AddDesc_EquipLevelCfg_2eproto();
  }
} static_descriptor_initializer_EquipLevelCfg_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int EquipLevelCfg::kLevelFieldNumber;
const int EquipLevelCfg::kExpFieldNumber;
#endif  // !_MSC_VER

EquipLevelCfg::EquipLevelCfg()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void EquipLevelCfg::InitAsDefaultInstance() {
}

EquipLevelCfg::EquipLevelCfg(const EquipLevelCfg& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void EquipLevelCfg::SharedCtor() {
  _cached_size_ = 0;
  level_ = 0u;
  exp_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

EquipLevelCfg::~EquipLevelCfg() {
  SharedDtor();
}

void EquipLevelCfg::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void EquipLevelCfg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const EquipLevelCfg& EquipLevelCfg::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_EquipLevelCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_EquipLevelCfg_2eproto();
#endif
  return *default_instance_;
}

EquipLevelCfg* EquipLevelCfg::default_instance_ = NULL;

EquipLevelCfg* EquipLevelCfg::New() const {
  return new EquipLevelCfg;
}

void EquipLevelCfg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    level_ = 0u;
    exp_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool EquipLevelCfg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 Level = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &level_)));
          set_has_level();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_Exp;
        break;
      }

      // required uint32 Exp = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Exp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &exp_)));
          set_has_exp();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void EquipLevelCfg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 Level = 1;
  if (has_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->level(), output);
  }

  // required uint32 Exp = 2;
  if (has_exp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->exp(), output);
  }

}

int EquipLevelCfg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 Level = 1;
    if (has_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->level());
    }

    // required uint32 Exp = 2;
    if (has_exp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->exp());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void EquipLevelCfg::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const EquipLevelCfg*>(&from));
}

void EquipLevelCfg::MergeFrom(const EquipLevelCfg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_level()) {
      set_level(from.level());
    }
    if (from.has_exp()) {
      set_exp(from.exp());
    }
  }
}

void EquipLevelCfg::CopyFrom(const EquipLevelCfg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool EquipLevelCfg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void EquipLevelCfg::Swap(EquipLevelCfg* other) {
  if (other != this) {
    std::swap(level_, other->level_);
    std::swap(exp_, other->exp_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string EquipLevelCfg::GetTypeName() const {
  return "com.cfg.vo.EquipLevelCfg";
}


// ===================================================================

#ifndef _MSC_VER
const int EquipLevelCfgSet::kEquiplevelcfgFieldNumber;
#endif  // !_MSC_VER

EquipLevelCfgSet::EquipLevelCfgSet()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void EquipLevelCfgSet::InitAsDefaultInstance() {
}

EquipLevelCfgSet::EquipLevelCfgSet(const EquipLevelCfgSet& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void EquipLevelCfgSet::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

EquipLevelCfgSet::~EquipLevelCfgSet() {
  SharedDtor();
}

void EquipLevelCfgSet::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void EquipLevelCfgSet::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const EquipLevelCfgSet& EquipLevelCfgSet::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_EquipLevelCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_EquipLevelCfg_2eproto();
#endif
  return *default_instance_;
}

EquipLevelCfgSet* EquipLevelCfgSet::default_instance_ = NULL;

EquipLevelCfgSet* EquipLevelCfgSet::New() const {
  return new EquipLevelCfgSet;
}

void EquipLevelCfgSet::Clear() {
  equiplevelcfg_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool EquipLevelCfgSet::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .com.cfg.vo.EquipLevelCfg equiplevelcfg = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_equiplevelcfg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_equiplevelcfg()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_equiplevelcfg;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void EquipLevelCfgSet::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .com.cfg.vo.EquipLevelCfg equiplevelcfg = 1;
  for (int i = 0; i < this->equiplevelcfg_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->equiplevelcfg(i), output);
  }

}

int EquipLevelCfgSet::ByteSize() const {
  int total_size = 0;

  // repeated .com.cfg.vo.EquipLevelCfg equiplevelcfg = 1;
  total_size += 1 * this->equiplevelcfg_size();
  for (int i = 0; i < this->equiplevelcfg_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->equiplevelcfg(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void EquipLevelCfgSet::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const EquipLevelCfgSet*>(&from));
}

void EquipLevelCfgSet::MergeFrom(const EquipLevelCfgSet& from) {
  GOOGLE_CHECK_NE(&from, this);
  equiplevelcfg_.MergeFrom(from.equiplevelcfg_);
}

void EquipLevelCfgSet::CopyFrom(const EquipLevelCfgSet& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool EquipLevelCfgSet::IsInitialized() const {

  for (int i = 0; i < equiplevelcfg_size(); i++) {
    if (!this->equiplevelcfg(i).IsInitialized()) return false;
  }
  return true;
}

void EquipLevelCfgSet::Swap(EquipLevelCfgSet* other) {
  if (other != this) {
    equiplevelcfg_.Swap(&other->equiplevelcfg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string EquipLevelCfgSet::GetTypeName() const {
  return "com.cfg.vo.EquipLevelCfgSet";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)
