// Copyright (c) 2014, Sailing Lab
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the <ORGANIZATION> nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "util/utils.hpp"

#include <utility>
#include <fstream>
#include <iostream>
#include <glog/logging.h>

#include "math.h"
#include <time.h>
#include <sys/sysinfo.h>
//#include "util/wtime.h"
//#include <Windows.h>

namespace petuum {

void GetHostInfos(std::string server_file,
  std::map<int32_t, HostInfo> *host_map) {
  std::map<int32_t, HostInfo>& servers = *host_map;

  std::ifstream input(server_file.c_str());
  std::string line;
  while(std::getline(input, line)){

    size_t pos = line.find_first_of("\t ");
    std::string idstr = line.substr(0, pos);

    size_t pos_ip = line.find_first_of("\t ", pos + 1);
    std::string ip = line.substr(pos + 1, pos_ip - pos - 1);
    std::string port = line.substr(pos_ip + 1);

    int32_t id =  atoi(idstr.c_str());
    servers.insert(std::make_pair(id, petuum::HostInfo(id, ip, port)));
    //VLOG(0) << "get server: " << id << ":" << ip << ":" << port;
  }
  input.close();
}

// assuming the namenode id is 0
void GetServerIDsFromHostMap(std::vector<int32_t> *server_ids,
    const std::map<int32_t, HostInfo>& host_map){
  
  int32_t num_servers = host_map.size() - 1;
  server_ids->resize(num_servers);
  int32_t i = 0;

  for (auto host_info_iter = host_map.cbegin();
    host_info_iter != host_map.cend(); host_info_iter++) {
    if (host_info_iter->first == 0)
      continue;
    (*server_ids)[i] = host_info_iter->first;
    ++i;
  }
}

//unsigned long threadID(const pthread_t &thrid)
//{
//	return (unsigned long)thrid.p;
//}

int32_t get_CPU_core_num()
{
#if defined(WIN32)  
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
#elif defined(LINUX) || defined(SOLARIS) || defined(AIX)  
	return get_nprocs();   //GNU fuction  
#else  
#error  unsupported system  
#endif  
}
}   // namespace petuum

namespace {
	const double cof[6] = { 76.18009172947146, -86.50532032941677,
		24.01409824083091, -1.231739572450155,
		0.1208650973866179e-2, -0.5395239384953e-5
	};
}

namespace lda {

	double LogGamma(double xx) {
		int j;
		double x, y, tmp1, ser;
		y = xx;
		x = xx;
		tmp1 = x + 5.5;
		tmp1 -= (x + 0.5)*log(tmp1);
		ser = 1.000000000190015;
		for (j = 0; j < 6; j++) ser += cof[j] / ++y;
		return -tmp1 + log(2.5066282746310005*ser / x);
	}

	double get_time() {
		struct timespec start;
		clock_gettime(CLOCK_MONOTONIC, &start);
		//petuum::clock_gettime(0, &start);
		return (start.tv_sec + start.tv_nsec / 1000000000.0);
	}

}
