//
// Created by chenzhi on 2023/5/8.
//

#include "message.pb.h"
#include <iostream>
#include <fstream>
#include <google/protobuf/util/json_util.h>

void convertToJSON(const MyMessage& message) {
    std::string json;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;

    google::protobuf::util::MessageToJsonString(message, &json, options);
    std::cout << "JSON representation of the message:\n" << json << std::endl;
}

void writeMessage(const std::string& filename) {
    MyMessage message;
    message.set_id(123);
    message.set_name("John Doe");

    std::string value;
    message.SerializeToString(&value);
    std::cout << "string====" << value.c_str() << std::endl;

    convertToJSON(message);

    std::ofstream output(filename, std::ios::binary);
    if (!message.SerializeToOstream(&output)) {
        std::cerr << "Failed to write message." << std::endl;
    }
}

void readMessage(const std::string& filename) {
    MyMessage message;

    std::ifstream input(filename, std::ios::binary);
    if (!message.ParseFromIstream(&input)) {
        std::cerr << "Failed to read message." << std::endl;
    }

    std::cout << "ID: " << message.id() << std::endl;
    std::cout << "Name: " << message.name() << std::endl;
}




int main() {
    std::string filename = "message.bin";

    // 写入消息
    writeMessage(filename);

    // 读取消息
    readMessage(filename);


    return 0;
}
