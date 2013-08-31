#if defined(__APPLE__) || defined(__CYGWIN__) || defined(WIN32)
// mtrace doesn't seem to exist on OSX, simply have this module do nothing
#define DISABLED
#endif

#include <v8.h>
#include <node.h>

#ifndef DISABLED
#include <mcheck.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "nan.h"

using namespace v8;
using namespace node;

static NAN_METHOD(GC) {
	NanScope();
	while (!V8::IdleNotification());
	NanReturnUndefined();
}

static NAN_METHOD(wrapMTrace) {
	NanScope();
#ifndef DISABLED
	const char *filename;
	std::string sfilename;
	char buf[64];
	if (args.Length() >= 1 && args[0]->IsString()) {
		// get filename
		String::Utf8Value utf8_value(args[0]);
		sfilename.assign(*utf8_value);
		filename = sfilename.c_str();
	} else {
		static int counter = 0;
		pid_t pid = getpid();
		long long int llpid = pid;
		sprintf(buf, "mtrace.%Ld.%d", llpid, counter++);
		filename = buf;
	}
	setenv("MALLOC_TRACE", filename, 1);

	mtrace();
	NanReturnValue(String::New(filename));
#else
	NanReturnUndefined();
#endif
}

static NAN_METHOD(wrapMUnTrace) {
	NanScope();
#ifndef DISABLED
	muntrace();
#endif
	NanReturnUndefined();
}

extern "C" {
  static void init(Handle<Object> module_exports) {
	NanScope();

	NODE_SET_METHOD(module_exports, "mtrace", wrapMTrace);
	NODE_SET_METHOD(module_exports, "muntrace", wrapMUnTrace);
	NODE_SET_METHOD(module_exports, "gc", GC);
  }

  NODE_MODULE(mtrace, init)
}
