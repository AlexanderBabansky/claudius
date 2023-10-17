#include <ai.h>
#include <vector>
#include <cstdio>

struct ParamStruct {
    const char* name = nullptr;
    const char* type = nullptr;
    const char* subtype = nullptr;
    bool default_bool = false;
    float default_float = 0;
    AtRGB default_rgb = AtRGB();
    int default_array_length = 0;
};

std::vector<ParamStruct> list_all_props(const AtNodeEntry* entry) {
    std::vector<ParamStruct> params;

    int entry_params_count = AiNodeEntryGetNumParams(entry);
    for (int p = 0; p < entry_params_count; p++) {
        const AtParamEntry* param = AiNodeEntryGetParameter(entry, p);
        ParamStruct str;
        str.name = AiParamGetName(param);
        str.type = AiParamGetTypeName(AiParamGetType(param));
        int subtype = AiParamGetSubType(param);
        if (subtype != 255) {
            str.subtype = AiParamGetTypeName(subtype);
        }

        if (AiParamGetType(param) == AI_TYPE_BOOLEAN) {
            str.default_bool = AiParamGetDefault(param)->BOOL();
        }
        if (AiParamGetType(param) == AI_TYPE_ARRAY) {
            AtArray* arr = AiParamGetDefault(param)->ARRAY();
            int num_elements = AiArrayGetNumElements(arr);
            str.default_array_length = num_elements;
        }
        if (AiParamGetType(param) == AI_TYPE_FLOAT) {
            str.default_float = AiParamGetDefault(param)->FLT();
        }
        if (AiParamGetType(param) == AI_TYPE_RGB) {
            str.default_rgb = AiParamGetDefault(param)->RGB();
        }
        params.push_back(str);
    }
    return params;
}

int main(int argc, char** argv) {
    AiBegin();
    if (argc != 2) {
        return 1;
    }
    const char* node_name = argv[1];
    auto entry = AiNodeEntryLookUp(node_name);
    if (!entry) {
        printf("Node %s not found\n", node_name);
        return 1;
    }
    auto params = list_all_props(entry);
    for (auto& p : params) {
        printf("%s\t%s\t%s\n", p.name, p.type, p.subtype);
    }
    AiEnd();
    return 0;
}
