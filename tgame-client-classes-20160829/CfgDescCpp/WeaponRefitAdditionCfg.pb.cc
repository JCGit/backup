// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WeaponRefitAdditionCfg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "WeaponRefitAdditionCfg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

void protobuf_ShutdownFile_WeaponRefitAdditionCfg_2eproto() {
  delete WeaponRefitAdditionCfg::default_instance_;
  delete WeaponRefitAdditionCfgSet::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  WeaponRefitAdditionCfg::default_instance_ = new WeaponRefitAdditionCfg();
  WeaponRefitAdditionCfgSet::default_instance_ = new WeaponRefitAdditionCfgSet();
  WeaponRefitAdditionCfg::default_instance_->InitAsDefaultInstance();
  WeaponRefitAdditionCfgSet::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_WeaponRefitAdditionCfg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto_once_);
void protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto_once_,
                 &protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_WeaponRefitAdditionCfg_2eproto {
  StaticDescriptorInitializer_WeaponRefitAdditionCfg_2eproto() {
    protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto();
  }
} static_descriptor_initializer_WeaponRefitAdditionCfg_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int WeaponRefitAdditionCfg::kQualityFieldNumber;
const int WeaponRefitAdditionCfg::kMultiFieldNumber;
#endif  // !_MSC_VER

WeaponRefitAdditionCfg::WeaponRefitAdditionCfg()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void WeaponRefitAdditionCfg::InitAsDefaultInstance() {
}

WeaponRefitAdditionCfg::WeaponRefitAdditionCfg(const WeaponRefitAdditionCfg& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void WeaponRefitAdditionCfg::SharedCtor() {
  _cached_size_ = 0;
  quality_ = 0u;
  multi_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

WeaponRefitAdditionCfg::~WeaponRefitAdditionCfg() {
  SharedDtor();
}

void WeaponRefitAdditionCfg::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void WeaponRefitAdditionCfg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const WeaponRefitAdditionCfg& WeaponRefitAdditionCfg::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto();
#endif
  return *default_instance_;
}

WeaponRefitAdditionCfg* WeaponRefitAdditionCfg::default_instance_ = NULL;

WeaponRefitAdditionCfg* WeaponRefitAdditionCfg::New() const {
  return new WeaponRefitAdditionCfg;
}

void WeaponRefitAdditionCfg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    quality_ = 0u;
    multi_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool WeaponRefitAdditionCfg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 Quality = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &quality_)));
          set_has_quality();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_Multi;
        break;
      }

      // required uint32 Multi = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Multi:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &multi_)));
          set_has_multi();
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

void WeaponRefitAdditionCfg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 Quality = 1;
  if (has_quality()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->quality(), output);
  }

  // required uint32 Multi = 2;
  if (has_multi()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->multi(), output);
  }

}

int WeaponRefitAdditionCfg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 Quality = 1;
    if (has_quality()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->quality());
    }

    // required uint32 Multi = 2;
    if (has_multi()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->multi());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void WeaponRefitAdditionCfg::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const WeaponRefitAdditionCfg*>(&from));
}

void WeaponRefitAdditionCfg::MergeFrom(const WeaponRefitAdditionCfg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_quality()) {
      set_quality(from.quality());
    }
    if (from.has_multi()) {
      set_multi(from.multi());
    }
  }
}

void WeaponRefitAdditionCfg::CopyFrom(const WeaponRefitAdditionCfg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool WeaponRefitAdditionCfg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void WeaponRefitAdditionCfg::Swap(WeaponRefitAdditionCfg* other) {
  if (other != this) {
    std::swap(quality_, other->quality_);
    std::swap(multi_, other->multi_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string WeaponRefitAdditionCfg::GetTypeName() const {
  return "com.cfg.vo.WeaponRefitAdditionCfg";
}


// ===================================================================

#ifndef _MSC_VER
const int WeaponRefitAdditionCfgSet::kWeaponrefitadditioncfgFieldNumber;
#endif  // !_MSC_VER

WeaponRefitAdditionCfgSet::WeaponRefitAdditionCfgSet()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void WeaponRefitAdditionCfgSet::InitAsDefaultInstance() {
}

WeaponRefitAdditionCfgSet::WeaponRefitAdditionCfgSet(const WeaponRefitAdditionCfgSet& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void WeaponRefitAdditionCfgSet::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

WeaponRefitAdditionCfgSet::~WeaponRefitAdditionCfgSet() {
  SharedDtor();
}

void WeaponRefitAdditionCfgSet::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void WeaponRefitAdditionCfgSet::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const WeaponRefitAdditionCfgSet& WeaponRefitAdditionCfgSet::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_WeaponRefitAdditionCfg_2eproto();
#endif
  return *default_instance_;
}

WeaponRefitAdditionCfgSet* WeaponRefitAdditionCfgSet::default_instance_ = NULL;

WeaponRefitAdditionCfgSet* WeaponRefitAdditionCfgSet::New() const {
  return new WeaponRefitAdditionCfgSet;
}

void WeaponRefitAdditionCfgSet::Clear() {
  weaponrefitadditioncfg_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool WeaponRefitAdditionCfgSet::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .com.cfg.vo.WeaponRefitAdditionCfg weaponrefitadditioncfg = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_weaponrefitadditioncfg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_weaponrefitadditioncfg()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_weaponrefitadditioncfg;
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

void WeaponRefitAdditionCfgSet::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .com.cfg.vo.WeaponRefitAdditionCfg weaponrefitadditioncfg = 1;
  for (int i = 0; i < this->weaponrefitadditioncfg_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->weaponrefitadditioncfg(i), output);
  }

}

int WeaponRefitAdditionCfgSet::ByteSize() const {
  int total_size = 0;

  // repeated .com.cfg.vo.WeaponRefitAdditionCfg weaponrefitadditioncfg = 1;
  total_size += 1 * this->weaponrefitadditioncfg_size();
  for (int i = 0; i < this->weaponrefitadditioncfg_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->weaponrefitadditioncfg(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void WeaponRefitAdditionCfgSet::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const WeaponRefitAdditionCfgSet*>(&from));
}

void WeaponRefitAdditionCfgSet::MergeFrom(const WeaponRefitAdditionCfgSet& from) {
  GOOGLE_CHECK_NE(&from, this);
  weaponrefitadditioncfg_.MergeFrom(from.weaponrefitadditioncfg_);
}

void WeaponRefitAdditionCfgSet::CopyFrom(const WeaponRefitAdditionCfgSet& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool WeaponRefitAdditionCfgSet::IsInitialized() const {

  for (int i = 0; i < weaponrefitadditioncfg_size(); i++) {
    if (!this->weaponrefitadditioncfg(i).IsInitialized()) return false;
  }
  return true;
}

void WeaponRefitAdditionCfgSet::Swap(WeaponRefitAdditionCfgSet* other) {
  if (other != this) {
    weaponrefitadditioncfg_.Swap(&other->weaponrefitadditioncfg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string WeaponRefitAdditionCfgSet::GetTypeName() const {
  return "com.cfg.vo.WeaponRefitAdditionCfgSet";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)