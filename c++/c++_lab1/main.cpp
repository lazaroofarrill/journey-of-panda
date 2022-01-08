#include <cMetaInterval.h>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

#include "cIntervalManager.h"
#include "cLerpNodePathInterval.h"

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

    NodePath pandaActor = window->load_model(framework.get_models(), "models/panda-model");
    pandaActor.set_scale(0.005);
    pandaActor.reparent_to(window->get_render());

    window->load_model(pandaActor, "models/panda-walk4");
    window->loop_animations(0);

    // Create the lerp intervals needed to walk back and forth
    PT(CLerpNodePathInterval) pandaPosInterval1, pandaPosInterval2,
            pandaHprInterval1, pandaHprInterval2;
    pandaPosInterval1 = new CLerpNodePathInterval("pandaPosInterval1",
                                                  13.0, CLerpInterval::BT_no_blend,
                                                  true, false, pandaActor, NodePath());
    pandaPosInterval1->set_start_pos(LPoint3(0, 10, 0));
    pandaPosInterval1->set_end_pos(LPoint3(0, -10, 0));

    pandaPosInterval2 = new CLerpNodePathInterval("pandaPosInterval2",
                                                  13.0, CLerpInterval::BT_no_blend,
                                                  true, false, pandaActor, NodePath());
    pandaPosInterval2->set_start_pos(LPoint3(0, -10, 0));
    pandaPosInterval2->set_end_pos(LPoint3(0, 10, 0));

    pandaHprInterval1 = new CLerpNodePathInterval("pandaHprInterval1", 3.0,
                                                  CLerpInterval::BT_no_blend,
                                                  true, false, pandaActor, NodePath());
    pandaHprInterval1->set_start_hpr(LPoint3(0, 0, 0));
    pandaHprInterval1->set_end_hpr(LPoint3(180, 0, 0));

    pandaHprInterval2 = new CLerpNodePathInterval("pandaHprInterval2", 3.0,
                                                  CLerpInterval::BT_no_blend,
                                                  true, false, pandaActor, NodePath());
    pandaHprInterval2->set_start_hpr(LPoint3(180, 0, 0));
    pandaHprInterval2->set_end_hpr(LPoint3(0, 0, 0));

    // Create and play the sequence that coordinates the intervals
    PT(CMetaInterval) pandaPace;
    pandaPace = new CMetaInterval("pandaPace");
    pandaPace->add_c_interval(pandaPosInterval1, 0,
                              CMetaInterval::RS_previous_end);
    pandaPace->add_c_interval(pandaHprInterval1, 0,
                              CMetaInterval::RS_previous_end);
    pandaPace->add_c_interval(pandaPosInterval2, 0,
                              CMetaInterval::RS_previous_end);
    pandaPace->add_c_interval(pandaHprInterval2, 0,
                              CMetaInterval::RS_previous_end);
    pandaPace->loop();


    //setup task
    taskMgr->add(new GenericAsyncTask("Spins the camera", &spinCameraTask, nullptr));

    Thread *current_thread = Thread::get_current_thread();
    while (framework.do_frame(current_thread)) {
        CIntervalManager::get_global_ptr()->step();
    }


    // Do the main loop, equal to run() in python
    framework.main_loop();
    framework.close_framework();
    return (0);
}