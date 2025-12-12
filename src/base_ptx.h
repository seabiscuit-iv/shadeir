#pragma once

#include <string>

const std::string base_ptx_start = R"(
.version 9.0
.target sm_89
.address_size 64

.visible .entry shadeir(

) {

.reg .pred exec_mask;


)";

const std::string base_ptx_end = R"(


}

)";

