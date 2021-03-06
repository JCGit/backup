// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CSItemOpMsg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "CSItemOpMsg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace vmsg {

void protobuf_ShutdownFile_CSItemOpMsg_2eproto() {
  delete CSOpenPackageRqst::default_instance_;
  delete CSOpenPackageRsp::default_instance_;
  delete CSItemRequest::default_instance_;
  delete CSItemRespond::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_CSItemOpMsg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_CSItemOpMsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  ::vmsg::protobuf_AddDesc_MsgBase_2eproto();
  ::vmsg::protobuf_AddDesc_ShareByClient_2eproto();
  ::vmsg::protobuf_AddDesc_CSEquipItemMsg_2eproto();
  ::vmsg::protobuf_AddDesc_CSBagMsg_2eproto();
  CSOpenPackageRqst::default_instance_ = new CSOpenPackageRqst();
  CSOpenPackageRsp::default_instance_ = new CSOpenPackageRsp();
  CSItemRequest::default_instance_ = new CSItemRequest();
  CSItemRespond::default_instance_ = new CSItemRespond();
  CSOpenPackageRqst::default_instance_->InitAsDefaultInstance();
  CSOpenPackageRsp::default_instance_->InitAsDefaultInstance();
  CSItemRequest::default_instance_->InitAsDefaultInstance();
  CSItemRespond::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_CSItemOpMsg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_CSItemOpMsg_2eproto_once_);
void protobuf_AddDesc_CSItemOpMsg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_CSItemOpMsg_2eproto_once_,
                 &protobuf_AddDesc_CSItemOpMsg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_CSItemOpMsg_2eproto {
  StaticDescriptorInitializer_CSItemOpMsg_2eproto() {
    protobuf_AddDesc_CSItemOpMsg_2eproto();
  }
} static_descriptor_initializer_CSItemOpMsg_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int CSOpenPackageRqst::kPackageIdxFieldNumber;
const int CSOpenPackageRqst::kPackageCfgIDFieldNumber;
#endif  // !_MSC_VER

CSOpenPackageRqst::CSOpenPackageRqst()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void CSOpenPackageRqst::InitAsDefaultInstance() {
}

CSOpenPackageRqst::CSOpenPackageRqst(const CSOpenPackageRqst& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void CSOpenPackageRqst::SharedCtor() {
  _cached_size_ = 0;
  packageidx_ = 0;
  packagecfgid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CSOpenPackageRqst::~CSOpenPackageRqst() {
  SharedDtor();
}

void CSOpenPackageRqst::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void CSOpenPackageRqst::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CSOpenPackageRqst& CSOpenPackageRqst::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CSItemOpMsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CSItemOpMsg_2eproto();
#endif
  return *default_instance_;
}

CSOpenPackageRqst* CSOpenPackageRqst::default_instance_ = NULL;

CSOpenPackageRqst* CSOpenPackageRqst::New() const {
  return new CSOpenPackageRqst;
}

void CSOpenPackageRqst::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    packageidx_ = 0;
    packagecfgid_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool CSOpenPackageRqst::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 PackageIdx = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &packageidx_)));
          set_has_packageidx();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_PackageCfgID;
        break;
      }

      // required uint32 PackageCfgID = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_PackageCfgID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &packagecfgid_)));
          set_has_packagecfgid();
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

void CSOpenPackageRqst::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 PackageIdx = 1;
  if (has_packageidx()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->packageidx(), output);
  }

  // required uint32 PackageCfgID = 2;
  if (has_packagecfgid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->packagecfgid(), output);
  }

}

int CSOpenPackageRqst::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 PackageIdx = 1;
    if (has_packageidx()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->packageidx());
    }

    // required uint32 PackageCfgID = 2;
    if (has_packagecfgid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->packagecfgid());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CSOpenPackageRqst::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CSOpenPackageRqst*>(&from));
}

void CSOpenPackageRqst::MergeFrom(const CSOpenPackageRqst& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_packageidx()) {
      set_packageidx(from.packageidx());
    }
    if (from.has_packagecfgid()) {
      set_packagecfgid(from.packagecfgid());
    }
  }
}

void CSOpenPackageRqst::CopyFrom(const CSOpenPackageRqst& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CSOpenPackageRqst::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void CSOpenPackageRqst::Swap(CSOpenPackageRqst* other) {
  if (other != this) {
    std::swap(packageidx_, other->packageidx_);
    std::swap(packagecfgid_, other->packagecfgid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CSOpenPackageRqst::GetTypeName() const {
  return "vmsg.CSOpenPackageRqst";
}


// ===================================================================

#ifndef _MSC_VER
const int CSOpenPackageRsp::kBagInfoFieldNumber;
const int CSOpenPackageRsp::kAwardLstFieldNumber;
#endif  // !_MSC_VER

CSOpenPackageRsp::CSOpenPackageRsp()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void CSOpenPackageRsp::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  baginfo_ = const_cast< ::vmsg::CSBagInfo*>(
      ::vmsg::CSBagInfo::internal_default_instance());
#else
  baginfo_ = const_cast< ::vmsg::CSBagInfo*>(&::vmsg::CSBagInfo::default_instance());
#endif
}

CSOpenPackageRsp::CSOpenPackageRsp(const CSOpenPackageRsp& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void CSOpenPackageRsp::SharedCtor() {
  _cached_size_ = 0;
  baginfo_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CSOpenPackageRsp::~CSOpenPackageRsp() {
  SharedDtor();
}

void CSOpenPackageRsp::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete baginfo_;
  }
}

void CSOpenPackageRsp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CSOpenPackageRsp& CSOpenPackageRsp::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CSItemOpMsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CSItemOpMsg_2eproto();
#endif
  return *default_instance_;
}

CSOpenPackageRsp* CSOpenPackageRsp::default_instance_ = NULL;

CSOpenPackageRsp* CSOpenPackageRsp::New() const {
  return new CSOpenPackageRsp;
}

void CSOpenPackageRsp::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_baginfo()) {
      if (baginfo_ != NULL) baginfo_->::vmsg::CSBagInfo::Clear();
    }
  }
  awardlst_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool CSOpenPackageRsp::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .vmsg.CSBagInfo BagInfo = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_baginfo()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_AwardLst;
        break;
      }

      // repeated .vmsg.CSAwardElement AwardLst = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_AwardLst:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_awardlst()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_AwardLst;
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

void CSOpenPackageRsp::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .vmsg.CSBagInfo BagInfo = 1;
  if (has_baginfo()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->baginfo(), output);
  }

  // repeated .vmsg.CSAwardElement AwardLst = 2;
  for (int i = 0; i < this->awardlst_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      2, this->awardlst(i), output);
  }

}

int CSOpenPackageRsp::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .vmsg.CSBagInfo BagInfo = 1;
    if (has_baginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->baginfo());
    }

  }
  // repeated .vmsg.CSAwardElement AwardLst = 2;
  total_size += 1 * this->awardlst_size();
  for (int i = 0; i < this->awardlst_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->awardlst(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CSOpenPackageRsp::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CSOpenPackageRsp*>(&from));
}

void CSOpenPackageRsp::MergeFrom(const CSOpenPackageRsp& from) {
  GOOGLE_CHECK_NE(&from, this);
  awardlst_.MergeFrom(from.awardlst_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_baginfo()) {
      mutable_baginfo()->::vmsg::CSBagInfo::MergeFrom(from.baginfo());
    }
  }
}

void CSOpenPackageRsp::CopyFrom(const CSOpenPackageRsp& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CSOpenPackageRsp::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_baginfo()) {
    if (!this->baginfo().IsInitialized()) return false;
  }
  for (int i = 0; i < awardlst_size(); i++) {
    if (!this->awardlst(i).IsInitialized()) return false;
  }
  return true;
}

void CSOpenPackageRsp::Swap(CSOpenPackageRsp* other) {
  if (other != this) {
    std::swap(baginfo_, other->baginfo_);
    awardlst_.Swap(&other->awardlst_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CSOpenPackageRsp::GetTypeName() const {
  return "vmsg.CSOpenPackageRsp";
}


// ===================================================================

#ifndef _MSC_VER
const int CSItemRequest::kOpenPackageRqstFieldNumber;
#endif  // !_MSC_VER

CSItemRequest::CSItemRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void CSItemRequest::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  openpackage_rqst_ = const_cast< ::vmsg::CSOpenPackageRqst*>(
      ::vmsg::CSOpenPackageRqst::internal_default_instance());
#else
  openpackage_rqst_ = const_cast< ::vmsg::CSOpenPackageRqst*>(&::vmsg::CSOpenPackageRqst::default_instance());
#endif
}

CSItemRequest::CSItemRequest(const CSItemRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void CSItemRequest::SharedCtor() {
  _cached_size_ = 0;
  openpackage_rqst_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CSItemRequest::~CSItemRequest() {
  SharedDtor();
}

void CSItemRequest::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete openpackage_rqst_;
  }
}

void CSItemRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CSItemRequest& CSItemRequest::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CSItemOpMsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CSItemOpMsg_2eproto();
#endif
  return *default_instance_;
}

CSItemRequest* CSItemRequest::default_instance_ = NULL;

CSItemRequest* CSItemRequest::New() const {
  return new CSItemRequest;
}

void CSItemRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_openpackage_rqst()) {
      if (openpackage_rqst_ != NULL) openpackage_rqst_->::vmsg::CSOpenPackageRqst::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool CSItemRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .vmsg.CSOpenPackageRqst OpenPackage_Rqst = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_openpackage_rqst()));
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

void CSItemRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .vmsg.CSOpenPackageRqst OpenPackage_Rqst = 1;
  if (has_openpackage_rqst()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->openpackage_rqst(), output);
  }

}

int CSItemRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .vmsg.CSOpenPackageRqst OpenPackage_Rqst = 1;
    if (has_openpackage_rqst()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->openpackage_rqst());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CSItemRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CSItemRequest*>(&from));
}

void CSItemRequest::MergeFrom(const CSItemRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_openpackage_rqst()) {
      mutable_openpackage_rqst()->::vmsg::CSOpenPackageRqst::MergeFrom(from.openpackage_rqst());
    }
  }
}

void CSItemRequest::CopyFrom(const CSItemRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CSItemRequest::IsInitialized() const {

  if (has_openpackage_rqst()) {
    if (!this->openpackage_rqst().IsInitialized()) return false;
  }
  return true;
}

void CSItemRequest::Swap(CSItemRequest* other) {
  if (other != this) {
    std::swap(openpackage_rqst_, other->openpackage_rqst_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CSItemRequest::GetTypeName() const {
  return "vmsg.CSItemRequest";
}


// ===================================================================

#ifndef _MSC_VER
const int CSItemRespond::kOpenPackageRspFieldNumber;
#endif  // !_MSC_VER

CSItemRespond::CSItemRespond()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void CSItemRespond::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  openpackage_rsp_ = const_cast< ::vmsg::CSOpenPackageRsp*>(
      ::vmsg::CSOpenPackageRsp::internal_default_instance());
#else
  openpackage_rsp_ = const_cast< ::vmsg::CSOpenPackageRsp*>(&::vmsg::CSOpenPackageRsp::default_instance());
#endif
}

CSItemRespond::CSItemRespond(const CSItemRespond& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void CSItemRespond::SharedCtor() {
  _cached_size_ = 0;
  openpackage_rsp_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CSItemRespond::~CSItemRespond() {
  SharedDtor();
}

void CSItemRespond::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete openpackage_rsp_;
  }
}

void CSItemRespond::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CSItemRespond& CSItemRespond::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CSItemOpMsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CSItemOpMsg_2eproto();
#endif
  return *default_instance_;
}

CSItemRespond* CSItemRespond::default_instance_ = NULL;

CSItemRespond* CSItemRespond::New() const {
  return new CSItemRespond;
}

void CSItemRespond::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_openpackage_rsp()) {
      if (openpackage_rsp_ != NULL) openpackage_rsp_->::vmsg::CSOpenPackageRsp::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool CSItemRespond::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .vmsg.CSOpenPackageRsp OpenPackage_Rsp = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_openpackage_rsp()));
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

void CSItemRespond::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .vmsg.CSOpenPackageRsp OpenPackage_Rsp = 1;
  if (has_openpackage_rsp()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->openpackage_rsp(), output);
  }

}

int CSItemRespond::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .vmsg.CSOpenPackageRsp OpenPackage_Rsp = 1;
    if (has_openpackage_rsp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->openpackage_rsp());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CSItemRespond::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CSItemRespond*>(&from));
}

void CSItemRespond::MergeFrom(const CSItemRespond& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_openpackage_rsp()) {
      mutable_openpackage_rsp()->::vmsg::CSOpenPackageRsp::MergeFrom(from.openpackage_rsp());
    }
  }
}

void CSItemRespond::CopyFrom(const CSItemRespond& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CSItemRespond::IsInitialized() const {

  if (has_openpackage_rsp()) {
    if (!this->openpackage_rsp().IsInitialized()) return false;
  }
  return true;
}

void CSItemRespond::Swap(CSItemRespond* other) {
  if (other != this) {
    std::swap(openpackage_rsp_, other->openpackage_rsp_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CSItemRespond::GetTypeName() const {
  return "vmsg.CSItemRespond";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vmsg

// @@protoc_insertion_point(global_scope)
