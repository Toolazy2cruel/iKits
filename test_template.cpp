//
// Created by chenzhi on 2023/3/16.
//

#include "test_template.h"
#include <iostream>

// 模板的定义
template <typename T>
TemplateClass<T>::TemplateClass() {
    // 构造函数的实现...
    std::cout << "TemplateClass init" << std::endl;
}

template <typename T>
void TemplateClass<T>::someFunction() {
    // 函数的实现...
    std::cout << "func init" << std::endl;
}

template class TemplateClass<int>;