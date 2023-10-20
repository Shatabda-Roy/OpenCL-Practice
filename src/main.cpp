#include "CL/cl.hpp"
#include <iostream>
#include <fstream>
#include <vector>
int main() {
	std::vector<cl::Platform> _platforms;
	cl::Platform::get(&_platforms);
	
	cl::Platform platform = _platforms.front();
	std::vector<cl::Device> _devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &_devices);

	auto _device = _devices.front();
	auto vendor = _device.getInfo<CL_DEVICE_VENDOR>();
	auto version = _device.getInfo<CL_DEVICE_VERSION>();

	std::ifstream helloWorldFile("src/HelloWorld.cl");
	std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));
	cl::Program::Sources _sources(1, std::make_pair(src.c_str(), src.length() + 1));
	cl::Context _context(_device);
	cl::Program _program(_context, _sources);
	auto err = _program.build("-cl-std=CL3.0");
	char _buffer[16];
	cl::Buffer _memBuf(_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(_buffer));
	cl::Kernel _kernel(_program, "HelloWorld", &err);
	_kernel.setArg(0, _memBuf);
	cl::CommandQueue _queue(_context, _device);
	_queue.enqueueNDRangeKernel(_kernel, NULL, 1, 1);
	_queue.enqueueReadBuffer(_memBuf, GL_TRUE, 0, sizeof(_buffer), _buffer);

	std::cout << _buffer;

	return EXIT_SUCCESS;
}