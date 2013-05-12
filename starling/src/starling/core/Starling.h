#if !defined(__STARLING_SRC_STARLING_CORE_STARLING_AS)
#define __STARLING_SRC_STARLING_CORE_STARLING_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash
{
    namespace display
    {
        class Sprite;
    }
}
namespace flash
{
    namespace display
    {
        class Stage3D;
    }
}
namespace flash
{
    namespace display
    {
        class StageAlign;
    }
}
namespace flash
{
    namespace display
    {
        class StageScaleMode;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3D;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3DCompareMode;
    }
}
namespace flash
{
    namespace display3D
    {
        class Context3DTriangleFace;
    }
}
namespace flash
{
    namespace display3D
    {
        class Program3D;
    }
}
namespace flash
{
    namespace errors
    {
        class IllegalOperationError;
    }
}
namespace flash
{
    namespace events
    {
        class ErrorEvent;
    }
}
namespace flash
{
    namespace events
    {
        class Event;
    }
}
namespace flash
{
    namespace events
    {
        class KeyboardEvent;
    }
}
namespace flash
{
    namespace events
    {
        class MouseEvent;
    }
}
namespace flash
{
    namespace events
    {
        class TouchEvent;
    }
}
namespace flash
{
    namespace geom
    {
        class Rectangle;
    }
}
namespace flash
{
    namespace system
    {
        class Capabilities;
    }
}
namespace flash
{
    namespace text
    {
        class TextField;
    }
}
namespace flash
{
    namespace text
    {
        class TextFieldAutoSize;
    }
}
namespace flash
{
    namespace text
    {
        class TextFormat;
    }
}
namespace flash
{
    namespace text
    {
        class TextFormatAlign;
    }
}
namespace flash
{
    namespace ui
    {
        class Mouse;
    }
}
namespace flash
{
    namespace ui
    {
        class Multitouch;
    }
}
namespace flash
{
    namespace ui
    {
        class MultitouchInputMode;
    }
}
namespace flash
{
    namespace utils
    {
        class ByteArray;
    }
}
namespace flash
{
    namespace utils
    {
        class Dictionary;
    }
}
namespace flash
{
    namespace utils
    {
        class getTimer;
    }
}
namespace flash
{
    namespace utils
    {
        class setTimeout;
    }
}

namespace starling
{
    namespace animation
    {
        class Juggler;
    }
}
namespace starling
{
    namespace display
    {
        class DisplayObject;
    }
}
namespace starling
{
    namespace display
    {
        class Stage;
    }
}
#include "starling/events/EventDispatcher.h"
namespace starling
{
    namespace events
    {
        class ResizeEvent;
    }
}
namespace starling
{
    namespace events
    {
        class TouchPhase;
    }
}
namespace starling
{
    namespace utils
    {
        class HAlign;
    }
}
namespace starling
{
    namespace utils
    {
        class VAlign;
    }
}

/** Dispatched when a new render context is created. */
//[Event(name="context3DCreate",type="starling.events.Event")]

/** Dispatched when the root class has been created. */
//[Event(name="rootCreated",type="starling.events.Event")]

































































































using namespace flash::display;
using namespace flash::display;
using namespace flash::display;
using namespace flash::display;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::display3D;
using namespace flash::errors;
using namespace flash::events;
using namespace flash::events;
using namespace flash::events;
using namespace flash::events;
using namespace flash::events;
using namespace flash::geom;
using namespace flash::system;
using namespace flash::text;
using namespace flash::text;
using namespace flash::text;
using namespace flash::text;
using namespace flash::ui;
using namespace flash::ui;
using namespace flash::ui;
using namespace flash::utils;
using namespace flash::utils;
using namespace flash::utils;
using namespace flash::utils;
using namespace starling::animation;
using namespace starling::display;
using namespace starling::display;
using namespace starling::events;
using namespace starling::events;
using namespace starling::events;
using namespace starling::utils;
using namespace starling::utils;

namespace starling
{
    namespace core
    {
        class/** The Starling class represents the core of the Starling framework.
     *
     *  <p>The Starling framework makes it possible to create 2D applications and games that make
     *  use of the Stage3D architecture introduced in Flash Player 11. It implements a display tree
     *  system that is very similar to that of conventional Flash, while leveraging modern GPUs
     *  to speed up rendering.</p>
     *
     *  <p>The Starling class represents the link between the conventional Flash display tree and
     *  the Starling display tree. To create a Starling-powered application, you have to create
     *  an instance of the Starling class:</p>
     *
     *  <pre>var starling:Starling = new Starling(Game, stage);</pre>
     *
     *  <p>The first parameter has to be a Starling display object class, e.g. a subclass of
     *  <code>starling.display.Sprite</code>. In the sample above, the class "Game" is the
     *  application root. An instance of "Game" will be created as soon as Starling is initialized.
     *  The second parameter is the conventional (Flash) stage object. Per default, Starling will
     *  display its contents directly below the stage.</p>
     *
     *  <p>It is recommended to store the Starling instance as a member variable, to make sure
     *  that the Garbage Collector does not destroy it. After creating the Starling object, you
     *  have to start it up like this:</p>
     *
     *  <pre>starling.start();</pre>
     *
     *  <p>It will now render the contents of the "Game" class in the frame rate that is set up for
     *  the application (as defined in the Flash stage).</p>
     *
     *  <strong>Accessing the Starling object</strong>
     *
     *  <p>From within your application, you can access the current Starling object anytime
     *  through the static method <code>Starling.current</code>. It will return the active Starling
     *  instance (most applications will only have one Starling object, anyway).</p>
     *
     *  <strong>Viewport</strong>
     *
     *  <p>The area the Starling content is rendered into is, per default, the complete size of the
     *  stage. You can, however, use the "viewPort" property to change it. This can be  useful
     *  when you want to render only into a part of the screen, or if the player size changes. For
     *  the latter, you can listen to the RESIZE-event dispatched by the Starling
     *  stage.</p>
     *
     *  <strong>Native overlay</strong>
     *
     *  <p>Sometimes you will want to display native Flash content on top of Starling. That's what the
     *  <code>nativeOverlay</code> property is for. It returns a Flash Sprite lying directly
     *  on top of the Starling content. You can add conventional Flash objects to that overlay.</p>
     *
     *  <p>Beware, though, that conventional Flash content on top of 3D content can lead to
     *  performance penalties on some (mobile) platforms. For that reason, always remove all child
     *  objects from the overlay when you don't need them any longer. Starling will remove the
     *  overlay from the display list when it's empty.</p>
     *
     *  <strong>Multitouch</strong>
     *
     *  <p>Starling supports multitouch input on devices that provide it. During development,
     *  where most of us are working with a conventional mouse and keyboard, Starling can simulate
     *  multitouch events with the help of the "Shift" and "Ctrl" (Mac: "Cmd") keys. Activate
     *  this feature by enabling the <code>simulateMultitouch</code> property.</p>
     *
     *  <strong>Handling a lost render context</strong>
     *
     *  <p>On some operating systems and under certain conditions (e.g. returning from system
     *  sleep), Starling's stage3D render context may be lost. Starling can recover from a lost
     *  context if the class property "handleLostContext" is set to "true". Keep in mind, however,
     *  that this comes at the price of increased memory consumption; Starling will cache textures
     *  in RAM to be able to restore them when the context is lost.</p>
     *
     *  <p>In case you want to react to a context loss, Starling dispatches an event with
     *  the type "Event.CONTEXT3D_CREATE" when the context is restored. You can recreate any
     *  invalid resources in a corresponding event listener.</p>
     *
     *  <strong>Sharing a 3D Context</strong>
     *
     *  <p>Per default, Starling handles the Stage3D context independently. If you want to combine
     *  Starling with another Stage3D engine, however, this may not be what you want. In this case,
     *  you can make use of the <code>shareContext</code> property:</p>
     *
     *  <ol>
     *    <li>Manually create and configure a context3D object that both frameworks can work with
     *        (through <code>stage3D.requestContext3D</code> and
     *        <code>context.configureBackBuffer</code>).</li>
     *    <li>Initialize Starling with the stage3D instance that contains that configured context.
     *        This will automatically enable <code>shareContext</code>.</li>
     *    <li>Call <code>start()</code> on your Starling instance (as usual). This will make
     *        Starling queue input events (keyboard/mouse/touch).</li>
     *    <li>Create a game loop (e.g. using the native <code>ENTER_FRAME</code> event) and let it
     *        call Starling's <code>nextFrame</code> as well as the equivalent method of the other
     *        Stage3D engine. Surround those calls with <code>context.clear()</code> and
     *        <code>context.present()</code>.</li>
     *  </ol>
     *
     *  <p>The Starling wiki contains a <a href="http://goo.gl/BsXzw">tutorial</a> with more
     *  information about this topic.</p>
     *
     */          Starling: public EventDispatcher
        {
        /** The version of the Starling framework. */public:
            static const std::string VERSION;

            /** The key for the shader programs stored in 'contextData' */
        private:
            static const std::string PROGRAM_DATA_NAME;

            // members

        private:
            Stage3D *mStage3D;
        private:
            Stage *mStage; // starling.display.stage!
        private:
            Class *mRootClass;
        private:
            DisplayObject *mRoot;
        private:
            Juggler *mJuggler;
        private:
            bool mStarted;
        private:
            RenderSupport *mSupport;
        private:
            TouchProcessor *mTouchProcessor;
        private:
            int mAntiAliasing;
        private:
            bool mSimulateMultitouch;
        private:
            bool mEnableErrorChecking;
        private:
            float mLastFrameTimestamp;
        private:
            bool mLeftMouseDown;
        private:
            StatsDisplay *mStatsDisplay;
        private:
            bool mShareContext;
        private:
            std::string mProfile;
        private:
            bool mSupportHighResolutions;
        private:
            Context3D *mContext;

        private:
            Rectangle *mViewPort;
        private:
            Rectangle *mPreviousViewPort;
        private:
            Rectangle *mClippedViewPort;

        private:
            flash::display::Stage *mNativeStage;
        private:
            flash::display::Sprite *mNativeOverlay;
        private:
            float mNativeStageContentScaleFactor;

        private:
            static  Starling *sCurrent;
        private:
            static  bool sHandleLostContext;
        private:
            static  std::map<std::string, void *> sContextData;

            // construction

            /** Creates a new Starling instance.
             *  @param rootClass  A subclass of a Starling display object. It will be created as soon as
             *                    initialization is finished and will become the first child of the
             *                    Starling stage.
             *  @param stage      The Flash (2D) stage.
             *  @param viewPort   A rectangle describing the area into which the content will be
             *                    rendered. @default stage size
             *  @param stage3D    The Stage3D object into which the content will be rendered. If it
             *                    already contains a context, <code>sharedContext</code> will be set
             *                    to <code>true</code>. @default the first available Stage3D.
             *  @param renderMode Use this parameter to force "software" rendering.
             *  @param profile    The Context3DProfile that should be requested.
             */
        public:
            Starling(Class *rootClass, flash::display::Stage *stage,
                     Rectangle *viewPort=NULL, Stage3D *stage3D=NULL,
                     std::string renderMode="auto", std::string profile="baselineConstrained");

            /** Disposes all children of the stage and the render context; removes all registered
             *  event listeners. */
        public:
            void     dispose();

            // functions

        private:
            void     initialize();

        private:
            void     initializeGraphicsAPI();

        private:
            void     initializeRoot();

            /** Calls <code>advanceTime()</code> (with the time that has passed since the last frame)
             *  and <code>render()</code>. */
        public:
            void     nextFrame();

            /** Dispatches ENTER_FRAME events on the display list, advances the Juggler
             *  and processes touches. */
        public:
            void     advanceTime(float passedTime);

            /** Renders the complete display list. Before rendering, the context is cleared; afterwards,
             *  it is presented. This can be avoided by enabling <code>shareContext</code>.*/
        public:
            void     render();

        private:
            void     updateViewPort(bool forceUpdate   =false);

        private:
            void     updateNativeOverlay();

        private:
            void     showFatalError(std::string message);

            /** Make this Starling instance the <code>current</code> one. */
        public:
            void     makeCurrent();

            /** As soon as Starling is started, it will queue input events (keyboard/mouse/touch);
             *  furthermore, the method <code>nextFrame</code> will be called once per Flash Player
             *  frame. (Except when <code>shareContext</code> is enabled: in that case, you have to
             *  call that method manually.) */
        public:
            void     start();

            /** Stops all logic processing and freezes the game in its current state. The content
             *  is still being rendered once per frame, though, because otherwise the conventional
             *  display list would no longer be updated. */
        public:
            void     stop();

            // event handlers

        private:
            void     onStage3DError(ErrorEvent *event);

        private:
            void     onContextCreated(Event *event);

        private:
            void     onEnterFrame(Event *event);

        private:
            void     onKey(KeyboardEvent *event);

        private:
            void     onResize(Event *event);

        private:
            void     onMouseLeave(Event *event);

        private:
            void     onTouch(Event *event);

        private:
            std::vector<void *> touchEventTypes();

            // program management

            /** Registers a vertex- and fragment-program under a certain name. If the name was already
             *  used, the previous program is overwritten. */
        public:
            Program3D *registerProgram(std::string name, ByteArray *vertexProgram,
                                       ByteArray *fragmentProgram);

            /** Deletes the vertex- and fragment-programs of a certain name. */
        public:
            void     deleteProgram(std::string name);

            /** Returns the vertex- and fragment-programs registered under a certain name. */
        public:
            Program3D *getProgram(std::string name);

            /** Indicates if a set of vertex- and fragment-programs is registered under a certain name. */
        public:
            bool     hasProgram(std::string name);

        private:
            std::map<std::string, void *> programs();

            // properties

            /** Indicates if a context is available and non-disposed. */
        private:
            bool         contextValid();

            /** Indicates if this Starling instance is started. */
        public:
            bool         isStarted();

            /** The default juggler of this instance. Will be advanced once per frame. */
        public:
            Juggler     *juggler();

            /** The render context of this instance. */
        public:
            Context3D   *context();

            /** A dictionary that can be used to save custom data related to the current context.
             *  If you need to share data that is bound to a specific stage3D instance
             *  (e.g. textures), use this dictionary instead of creating a static class variable.
             *  The Dictionary is actually bound to the stage3D instance, thus it survives a
             *  context loss. */
        public:
            std::map<std::string, void *> contextData();

            /** Indicates if multitouch simulation with "Shift" and "Ctrl"/"Cmd"-keys is enabled.
             *  @default false */
        public:
            bool         simulateMultitouch();
        public:
            void         simulateMultitouch(bool value);

            /** Indicates if Stage3D render methods will report errors. Activate only when needed,
             *  as this has a negative impact on performance. @default false */
        public:
            bool         enableErrorChecking();
        public:
            void         enableErrorChecking(bool value);

            /** The antialiasing level. 0 - no antialasing, 16 - maximum antialiasing. @default 0 */
        public:
            int          antiAliasing();
        public:
            void         antiAliasing(int value);

            /** The viewport into which Starling contents will be rendered. */
        public:
            Rectangle   *viewPort();
        public:
            void         viewPort(Rectangle *value);

            /** The ratio between viewPort width and stage width. Useful for choosing a different
             *  set of textures depending on the display resolution. */
        public:
            float        contentScaleFactor();

            /** A Flash Sprite placed directly on top of the Starling content. Use it to display native
             *  Flash components. */
        public:
            Sprite      *nativeOverlay();

            /** Indicates if a small statistics box (with FPS, memory usage and draw count) is displayed. */
        public:
            bool         showStats();
        public:
            void         showStats(bool value);

            /** Displays the statistics box at a certain position. */
        public:
            void     showStatsAt(std::string hAlign="left", std::string vAlign="top", float scale =1);

            /** The Starling stage object, which is the root of the display tree that is rendered. */
        public:
            Stage       *stage();

            /** The Flash Stage3D object Starling renders into. */
        public:
            Stage3D     *stage3D();

            /** The Flash (2D) stage object Starling renders beneath. */
        public:
            flash::display::Stage *nativeStage();

            /** The instance of the root class provided in the constructor. Available as soon as
             *  the event 'ROOT_CREATED' has been dispatched. */
        public:
            DisplayObject *root();

            /** Indicates if the Context3D render calls are managed externally to Starling,
             *  to allow other frameworks to share the Stage3D instance. @default false */
        public:
            bool         shareContext();
        public:
            void         shareContext(bool value);

            /** The Context3D profile as requested in the constructor. Beware that if you are
             *  using a shared context, this might not be accurate. */
        public:
            std::string  profile();

            /** Indicates that if the device supports HiDPI screens Starling will attempt to allocate
             *  a larger back buffer than indicated via the viewPort size. Note that this is used
             *  on Desktop only; mobile AIR apps still use the "requestedDisplayResolution" parameter
             *  the application descriptor XML. */
        public:
            bool         supportHighResolutions();
        public:
            void         supportHighResolutions(bool value);

            // static properties

            /** The currently active Starling instance. */
        public:
            static Starling *current();

            /** The render context of the currently active Starling instance. */
        public:
            static Context3D *context();

            /** The default juggler of the currently active Starling instance. */
        public:
            static Juggler *juggler();

            /** The contentScaleFactor of the currently active Starling instance. */
        public:
            static float contentScaleFactor();

            /** Indicates if multitouch input should be supported. */
        public:
            static bool  multitouchEnabled();

        public:
            static void  multitouchEnabled(bool value);

            /** Indicates if Starling should automatically recover from a lost device context.
             *  On some systems, an upcoming screensaver or entering sleep mode may
             *  invalidate the render context. This setting indicates if Starling should recover from
             *  such incidents. Beware that this has a huge impact on memory consumption!
             *  It is recommended to enable this setting on Android and Windows, but to deactivate it
             *  on iOS and Mac OS X. @default false */
        public:
            static bool  handleLostContext();
        public:
            static void  handleLostContext(bool value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_CORE_STARLING_AS
#endif // __cplusplus

