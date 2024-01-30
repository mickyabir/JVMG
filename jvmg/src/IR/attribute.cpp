//
// Created by Micky on 1/24/2024.
//
#include "jvmg/IR/attribute.h"

using namespace jvmg;

std::map<std::string, AttributeInfo::AttributeNameTag> AttributeInfo::attributeNameTagMap = {
        {"ConstantValue", AttributeInfo::CONSTANT_VALUE},
        {"Code", AttributeInfo::CODE},
        {"StackMapTable", AttributeInfo::STACK_MAP_TABLE},
        {"Exceptions", AttributeInfo::EXCEPTIONS},
        {"InnerClasses", AttributeInfo::INNER_CLASSES},
        {"EnclosingMethod", AttributeInfo::ENCLOSING_METHOD},
        {"Synthetic", AttributeInfo::SYNTHETIC},
        {"Signature", AttributeInfo::SIGNATURE},
        {"SourceFile", AttributeInfo::SOURCE_FILE},
        {"SourceDebugExtension", AttributeInfo::SOURCE_DEBUG_EXTENSION},
        {"LineNumberTable", AttributeInfo::LINE_NUMBER_TABLE},
        {"LocalVariableTable", AttributeInfo::LOCAL_VARIABLE_TABLE},
        {"LocalVariableTypeTable", AttributeInfo::LOCAL_VARIABLE_TYPE_TABLE},
        {"Deprecated", AttributeInfo::DEPRECATED},
        {"RuntimeVisibleAnnotations", AttributeInfo::RUNTIME_VISIBLE_ANNOTATIONS},
        {"RuntimeInvisibleAnnotations", AttributeInfo::RUNTIME_INVISIBLE_ANNOTATIONS},
        {"RuntimeVisibleParamterAnnotations", AttributeInfo::RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS},
        {"RuntimeInvisibleParameterAnnotations", AttributeInfo::RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS},
        {"AnnotationDefault", AttributeInfo::ANNOTATION_DEFAULT},
        {"BootstrapMethods", AttributeInfo::BOOTSTRAP_METHODS}
};


void CodeAttribute::_serialize() {
    serializeBytes(maxStack);
    serializeBytes(maxLocals);
    serializeBytes(codeLength);

    for (auto& inst : code) {
        insertBytes(inst.serialize());
    }

    serializeBytes(exceptionTableLength);
    for (auto& exception : exceptionTable) {
        insertBytes(exception.serialize());
    }

    serializeBytes(attributesCount);
    for (auto& attribute : attributes) {
        insertBytes(attribute->serialize());
    }
}