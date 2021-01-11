#include "Context.h"
#include "Log.h"
namespace GEngine {
	
	void Context::init() {
		glfwMakeContextCurrent(glWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to initialize Glad!");
		GE_CORE_INFO("OpenGL Info:");
		GE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void Context::swapBuffers() {
		glfwSwapBuffers(glWindow);
	}

}