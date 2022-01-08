#include "pandaFramework.h"
#include "pandaSystem.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

// The global task manager
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
// The global clock
PT(ClockObject) globalClock = ClockObject::get_global_clock();
// Here's what we'll store the camera in.
NodePath camera;

AsyncTask::DoneStatus spinCameraTask(GenericAsyncTask *task, void *data) {
    // Calculate the new position and orientation (inefficient - change me!)
    double time = globalClock->get_real_time();
    double angleDegrees = time * 6.0;
    double angleRadians = angleDegrees * (3.14 / 180.0);
    camera.set_pos(20 * sin(angleRadians), -20.0 * cos(angleRadians), 3);
    camera.set_hpr(angleDegrees, 0, 0);

    // Tell the task manager to continue this task the next frame.
    return AsyncTask::DS_cont;
}

int main(int argc, char *argv[]) {
    // Open a new window framework
    PandaFramework framework;
    framework.open_framework(argc, argv);

    // Set the window title and open the window
    framework.set_window_title("My Panda3D Window");
    WindowFramework *window = framework.open_window();
    window->setup_trackball();
    camera = window->get_camera_group();

    // Here is room for your own code

    NodePath scene = window->load_model(framework.get_models(), "models/environment");
    scene.reparent_to(window->get_render());
    scene.set_scale(0.25f, 0.25f, 0.25f);
    scene.set_pos(-8, 42, 0);

    taskMgr->add(new GenericAsyncTask("Spins the camera", &spinCameraTask, nullptr));


    // Do the main loop, equal to run() in python
    framework.main_loop();
    framework.close_framework();
    return (0);
}