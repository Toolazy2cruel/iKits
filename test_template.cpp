//
// Created by chenzhi on 2023/3/16.
//

#include "test_template.h"
#include <iostream>

template<class T>
void TestTemplate<T>::f()
{
    std::cout << "template defined v1!" << std::endl;

}

void TestTemplateV2() {
    std::cout << "template defined v2!" << std::endl;
}
template class TestTemplate<int>;
