// =================================================================================================
//
//  Starling Framework
//  Copyright 2012 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "Starling.h"
#include "flash/display/Sprite.h"
#include "flash/display/Stage3D.h"
#include "flash/display/StageAlign.h"
#include "flash/display/StageScaleMode.h"
#include "flash/display3D/Context3D.h"
#include "flash/display3D/Context3DCompareMode.h"
#include "flash/display3D/Context3DTriangleFace.h"
#include "flash/display3D/Program3D.h"
#include "flash/errors/IllegalOperationError.h"
#include "flash/events/ErrorEvent.h"
#include "flash/events/Event.h"
#include "flash/events/KeyboardEvent.h"
#include "flash/events/MouseEvent.h"
#include "flash/events/TouchEvent.h"
#include "flash/geom/Rectangle.h"
#include "flash/system/Capabilities.h"
#include "flash/text/TextField.h"
#include "flash/text/TextFieldAutoSize.h"
#include "flash/text/TextFormat.h"
#include "flash/text/TextFormatAlign.h"
#include "flash/ui/Mouse.h"
#include "flash/ui/Multitouch.h"
#include "flash/ui/MultitouchInputMode.h"
#include "flash/utils/ByteArray.h"
#include "flash/utils/Dictionary.h"
#include "flash/utils/getTimer.h"
#include "flash/utils/setTimeout.h"

#include "starling/animation/Juggler.h"
#include "starling/display/DisplayObject.h"
#include "starling/display/Stage.h"
#include "starling/events/EventDispatcher.h"
#include "starling/events/ResizeEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/utils/HAlign.h"
#include "starling/utils/VAlign.h"

/** Dispatched when a new render context is created. */





































































































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
        /** The Starling class represents the core of the Starling framework.
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
         */ /** The version of the Starling framework. */const std::string Starling::VERSION="1.3";

        /** The key for the shader programs stored in 'contextData' */
        const std::string Starling::PROGRAM_DATA_NAME="Starling.programs";

        // members                                  // starling.display.stage!

        Starling *Starling::sCurrent;
        bool Starling::sHandleLostContext;
        std::map<std::string, void *> Starling::sContextData=newDictionary(true);

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
        Starling::Starling(Class *rootClass, flash::display::Stage *stage,
                           Rectangle *viewPort, Stage3D *stage3D,
                           std::string renderMode, std::string profile)
        {
            if (stage == NULL) throw new ArgumentError("Stage must not be null");
            if (rootClass == NULL) throw new ArgumentError("Root class must not be null");
            if (viewPort == NULL) viewPort = new Rectangle(0, 0, stage->stageWidth,stage->stageHeight);
            if (stage3D == NULL) stage3D = stage->stage3Ds[0];

            makeCurrent();

            mRootClass = rootClass;
            mViewPort = viewPort;
            mPreviousViewPort = new Rectangle();
            mStage3D = stage3D;
            mStage = new Stage(viewPort->width,viewPort->height,stage->color);
            mNativeOverlay = new Sprite();
            mNativeStage = stage;
            mNativeStage->addChild(mNativeOverlay);
            mNativeStageContentScaleFactor = 1.0;
            mTouchProcessor = new TouchProcessor(mStage);
            mJuggler = new Juggler();
            mAntiAliasing = 0;
            mSimulateMultitouch = false;
            mEnableErrorChecking = false;
            mProfile = profile;
            mSupportHighResolutions = false;
            mLastFrameTimestamp = getTimer() / 1000.0;
            mSupport  = new RenderSupport();

            // for context data, we actually reference by stage3D, since it survives a context loss
            sContextData[stage3D] = new Dictionary();
            sContextData[stage3D][PROGRAM_DATA_NAME] = new Dictionary();

            // all other modes are problematic in Starling, so we force those here
            stage->scaleMode= StageScaleMode::NO_SCALE;
            stage->align= StageAlign::TOP_LEFT;

            // register touch/mouse event handlers
            for each (var std::string touchEventTypeintouchEventTypes)
                stage->addEventListener(touchEventType,onTouch, false, 0, true);

            // register other event handlers
            stage->addEventListener(Event::ENTER_FRAME,onEnterFrame,false, 0, true);
            stage->addEventListener(KeyboardEvent::KEY_DOWN,onKey,false, 0, true);
            stage->addEventListener(KeyboardEvent::KEY_UP,onKey,false, 0, true);
            stage->addEventListener(Event::RESIZE,onResize,false, 0, true);
            stage->addEventListener(Event::MOUSE_LEAVE,onMouseLeave,false, 0, true);

            mStage3D->addEventListener(Event::CONTEXT3D_CREATE,onContextCreated,false, 10, true);
            mStage3D->addEventListener(ErrorEvent::ERROR,onStage3DError,false, 10, true);

            if (mStage3D->context3D&& mStage3D->context3D->driverInfo!="Disposed")
            {
                mShareContext = true;
                setTimeout(initialize, 1); // we don't call it right away, because Starling should
                // behave the same way with or without a shared context
            }
            else
            {
                mShareContext = false;

                try
                {
                    // "Context3DProfile" is only available starting with Flash Player 11.4/AIR 3.4.
                    // to stay compatible with older versions, we check if the parameter is available.

                    Function *requestContext3D=mStage3D->requestContext3D;
                    if (requestContext3D->length== 1) requestContext3D(renderMode);
                    else requestContext3D(renderMode, profile);
                }
catch (e:Error *)
                {
                    showFatalError("Context3D error: " + e->message);
                }
            }
        }

        /** Disposes all children of the stage and the render context; removes all registered
         *  event listeners. */
        void Starling::dispose()
        {
            stop();

            mNativeStage->removeEventListener(Event::ENTER_FRAME,onEnterFrame,false);
            mNativeStage->removeEventListener(KeyboardEvent::KEY_DOWN,onKey,false);
            mNativeStage->removeEventListener(KeyboardEvent::KEY_UP,onKey,false);
            mNativeStage->removeEventListener(Event::RESIZE,onResize,false);
            mNativeStage->removeEventListener(Event::MOUSE_LEAVE,onMouseLeave,false);
            mNativeStage->removeChild(mNativeOverlay);

            mStage3D->removeEventListener(Event::CONTEXT3D_CREATE,onContextCreated,false);
            mStage3D->removeEventListener(ErrorEvent::ERROR,onStage3DError,false);

            for each (var std::string touchEventTypeintouchEventTypes)
                mNativeStage->removeEventListener(touchEventType,onTouch, false);

            if (mStage) mStage->dispose();
            if (mSupport) mSupport->dispose();
            if (mTouchProcessor) mTouchProcessor->dispose();
            if (sCurrent == this) sCurrent = NULL;
            if (mContext && !mShareContext)
            {
                // Per default, the context is recreated as long as there are listeners on it.
                // Beginning with AIR 3.6, we can avoid that with an additional parameter.

                Function *disposeContext3D=mContext->dispose;
                if (disposeContext3D->length== 1) disposeContext3D(false);
                else disposeContext3D();
            }
        }

        // functions

        void Starling::initialize()
        {
            makeCurrent();

            initializeGraphicsAPI();
            initializeRoot();

            mTouchProcessor->simulateMultitouch= mSimulateMultitouch;
            mLastFrameTimestamp = getTimer() / 1000.0;
        }

        void Starling::initializeGraphicsAPI()
        {
            mContext = mStage3D->context3D;
            mContext->enableErrorChecking= mEnableErrorChecking;
            contextData[PROGRAM_DATA_NAME] = new Dictionary();

            updateViewPort(true);

            trace("[Starling] Initialization complete.");
            trace("[Starling] Display Driver:", mContext->driverInfo);

            dispatchEventWith(starling->events->Event::CONTEXT3D_CREATE,false,mContext);
        }

        void Starling::initializeRoot()
        {
            if (mRoot == NULL)
            {
                mRoot = new mRootClass() as DisplayObject;
                if (mRoot == NULL) throw new Error("Invalid root class: " + mRootClass);
                mStage->addChildAt(mRoot,0);

                dispatchEventWith(starling->events->Event::ROOT_CREATED,false,mRoot);
            }
        }

        /** Calls <code>advanceTime()</code> (with the time that has passed since the last frame)
         *  and <code>render()</code>. */
        void Starling::nextFrame()
        {
            float now = getTimer() / 1000.0;
            float passedTime = now - mLastFrameTimestamp;
            mLastFrameTimestamp = now;

            advanceTime(passedTime);
            render();
        }

        /** Dispatches ENTER_FRAME events on the display list, advances the Juggler
         *  and processes touches. */
        void Starling::advanceTime(float passedTime)
        {
            makeCurrent();

            mTouchProcessor->advanceTime(passedTime);
            mStage->advanceTime(passedTime);
            mJuggler->advanceTime(passedTime);
        }

        /** Renders the complete display list. Before rendering, the context is cleared; afterwards,
         *  it is presented. This can be avoided by enabling <code>shareContext</code>.*/
        void Starling::render()
        {
            if (!contextValid)
                return;

            makeCurrent();
            updateViewPort();
            updateNativeOverlay();
            mSupport->nextFrame();

            if (!mShareContext)
                RenderSupport->clear(mStage->color,1.0);

            float scaleX = mViewPort->width / mStage->stageWidth;
            float scaleY = mViewPort->height/ mStage->stageHeight;

            mContext->setDepthTest(false,Context3DCompareMode::ALWAYS);
            mContext->setCulling(Context3DTriangleFace::NONE);

            mSupport->renderTarget= NULL; // back buffer
            mSupport->setOrthographicProjection(
                mViewPort->x< 0 ? -mViewPort->x/ scaleX : 0.0,
                mViewPort->y< 0 ? -mViewPort->y/ scaleY : 0.0,
                mClippedViewPort->width / scaleX,
                mClippedViewPort->height/ scaleY);

            mStage->render(mSupport,1.0);
            mSupport->finishQuadBatch();

            if (mStatsDisplay)
                mStatsDisplay->drawCount= mSupport->drawCount;

            if (!mShareContext)
                mContext->present();
        }

        void Starling::updateViewPort(bool forceUpdate)
        {
            // the last set viewport is stored in a variable; that way, people can modify the
            // viewPort directly (without a copy) and we still know if it has changed.

            if (forceUpdate || mPreviousViewPort->width!= mViewPort->width||
                    mPreviousViewPort->height!= mViewPort->height||
                    mPreviousViewPort->x!= mViewPort->x|| mPreviousViewPort->y!= mViewPort->y)
            {
                mPreviousViewPort->setTo(mViewPort->x,mViewPort->y,mViewPort->width,mViewPort->height);

                // Constrained mode requires that the viewport is within the native stage bounds;
                // thus, we use a clipped viewport when configuring the back buffer. (In baseline
                // mode, that's not necessary, but it does not hurt either.)

                mClippedViewPort = mViewPort->intersection(
                                       new Rectangle(0, 0, mNativeStage->stageWidth,mNativeStage->stageHeight));

                if (!mShareContext)
                {
                    // setting x and y might move the context to invalid bounds (since changing
                    // the size happens in a separate operation) -- so we have no choice but to
                    // set the backbuffer to a very small size first, to be on the safe side.

                    if (mProfile == "baselineConstrained")
                        mSupport->configureBackBuffer(32,32, mAntiAliasing, false);

                    mStage3D->x= mClippedViewPort->x;
                    mStage3D->y= mClippedViewPort->y;

                    mSupport->configureBackBuffer(mClippedViewPort->width,mClippedViewPort->height,
                                                  mAntiAliasing, false, mSupportHighResolutions);

                    if (mSupportHighResolutions && "contentsScaleFactor" in mNativeStage)
                        mNativeStageContentScaleFactor = mNativeStage["contentsScaleFactor"];
                    else
                        mNativeStageContentScaleFactor = 1.0;
                }
                else
                {
                    mSupport->backBufferWidth = mClippedViewPort->width;
                    mSupport->backBufferHeight= mClippedViewPort->height;
                }
            }
        }

        void Starling::updateNativeOverlay()
        {
            mNativeOverlay->x= mViewPort->x;
            mNativeOverlay->y= mViewPort->y;
            mNativeOverlay->scaleX= mViewPort->width/ mStage->stageWidth;
            mNativeOverlay->scaleY= mViewPort->height/ mStage->stageHeight;
        }

        void Starling::showFatalError(std::string message)
        {
            TextField *textField=new TextField();
            TextFormat *textFormat=new TextFormat("Verdana", 12, 0xFFFFFF);
            textFormat->align= TextFormatAlign::CENTER;
            textField->defaultTextFormat= textFormat;
            textField->wordWrap= true;
            textField->width= mStage->stageWidth* 0.75;
            textField->autoSize= TextFieldAutoSize::CENTER;
            textField->text= message;
            textField->x= (mStage->stageWidth- textField->width)/ 2;
            textField->y= (mStage->stageHeight- textField->height)/ 2;
            textField->background= true;
            textField->backgroundColor= 0x440000;
            nativeOverlay->addChild(textField);
        }

        /** Make this Starling instance the <code>current</code> one. */
        void Starling::makeCurrent()
        {
            sCurrent = this;
        }

        /** As soon as Starling is started, it will queue input events (keyboard/mouse/touch);
         *  furthermore, the method <code>nextFrame</code> will be called once per Flash Player
         *  frame. (Except when <code>shareContext</code> is enabled: in that case, you have to
         *  call that method manually.) */
        void Starling::start()
        {
            mStarted = true;
            mLastFrameTimestamp = getTimer() / 1000.0;
        }

        /** Stops all logic processing and freezes the game in its current state. The content
         *  is still being rendered once per frame, though, because otherwise the conventional
         *  display list would no longer be updated. */
        void Starling::stop()
        {
            mStarted = false;
        }

        // event handlers

        void Starling::onStage3DError(ErrorEvent *event)
        {
            if (event->errorID== 3702)
            {
                std::string mode=Capabilities::playerType=="Desktop"?"renderMode": "wmode";
                showFatalError("Context3D not available! Possible reasons: wrong " + mode +
                               " or missing device support.");
            }
            else
                showFatalError("Stage3D error: " + event->text);
        }

        void Starling::onContextCreated(Event *event)
        {
            if (!Starling->handleLostContext&& mContext)
            {
                stop();
                event->stopImmediatePropagation();
                showFatalError("Fatal error: The application lost the device context!");
                trace("[Starling] The device context was lost. " +
                      "Enable 'Starling.handleLostContext' to avoid this error.");
            }
            else
            {
                initialize();
            }
        }

        void Starling::onEnterFrame(Event *event)
        {
            // On mobile, the native display list is only updated on stage3D draw calls.
            // Thus, we render even when Starling is paused.

            if (!mShareContext)
            {
                if (mStarted) nextFrame();
                else          render();
            }
        }

        void Starling::onKey(KeyboardEvent *event)
        {
            if (!mStarted) return;

            makeCurrent();
            mStage->dispatchEvent(newstarling->events->KeyboardEvent(
                                      event->type,event->charCode,event->keyCode,event->keyLocation,
                                      event->ctrlKey,event->altKey,event->shiftKey));
        }

        void Starling::onResize(Event *event)
        {
            flash::display::Stage *stage=event->targetasflash->display->Stage;
            mStage->dispatchEvent(newResizeEvent(Event::RESIZE,stage->stageWidth,stage->stageHeight));
        }

        void Starling::onMouseLeave(Event *event)
        {
            mTouchProcessor->enqueueMouseLeftStage();
        }

        void Starling::onTouch(Event *event)
        {
            if (!mStarted) return;

            float globalX;
            float globalY;
            int touchID;
            std::string phase;
            float pressure = 1.0;
            float width = 1.0;
            float height = 1.0;

            // figure out general touch properties
            if (dynamic_cast<MouseEvent>(event))
            {
                MouseEvent *mouseEvent=static_cast<MouseEvent>(event);
                globalX = mouseEvent->stageX;
                globalY = mouseEvent->stageY;
                touchID = 0;

                // MouseEvent.buttonDown returns true for both left and right button (AIR supports
                // the right mouse button). We only want to react on the left button for now,
                // so we have to save the state for the left button manually.
                if (event->type== MouseEvent::MOUSE_DOWN)   mLeftMouseDown = true;
                else if (event->type== MouseEvent::MOUSE_UP)mLeftMouseDown = false;
            }
            else
            {
                TouchEvent *touchEvent=static_cast<TouchEvent>(event);
                globalX = touchEvent->stageX;
                globalY = touchEvent->stageY;
                touchID = touchEvent->touchPointID;
                pressure = touchEvent->pressure;
                width = touchEvent->sizeX;
                height = touchEvent->sizeY;
            }

            // figure out touch phase
            switch (event->type)
            {
                case TouchEvent::TOUCH_BEGIN:
                    phase = TouchPhase::BEGAN;
                    break;
                case TouchEvent::TOUCH_MOVE:
                    phase = TouchPhase::MOVED;
                    break;
                case TouchEvent::TOUCH_END:
                    phase = TouchPhase::ENDED;
                    break;
                case MouseEvent::MOUSE_DOWN:
                    phase = TouchPhase::BEGAN;
                    break;
                case MouseEvent::MOUSE_UP:
                    phase = TouchPhase::ENDED;
                    break;
                case MouseEvent::MOUSE_MOVE:
                    phase = (mLeftMouseDown ? TouchPhase::MOVED: TouchPhase::HOVER);
                    break;
            }

            // move position into viewport bounds
            globalX = mStage->stageWidth * (globalX - mViewPort->x)/ mViewPort->width;
            globalY = mStage->stageHeight* (globalY - mViewPort->y)/ mViewPort->height;

            // enqueue touch in touch processor
            mTouchProcessor->enqueue(touchID,phase, globalX, globalY, pressure, width, height);
        }

        std::vector<void *> Starling::touchEventTypes()
        {
            return Mouse::supportsCursor|| !multitouchEnabled ?
                   [ MouseEvent::MOUSE_DOWN, MouseEvent::MOUSE_MOVE,MouseEvent::MOUSE_UP] :
                   [ TouchEvent::TOUCH_BEGIN,TouchEvent::TOUCH_MOVE,TouchEvent::TOUCH_END];
        }

        // program management

        /** Registers a vertex- and fragment-program under a certain name. If the name was already
         *  used, the previous program is overwritten. */
        Program3D *Starling::registerProgram(std::string name, ByteArray *vertexProgram,
                                             ByteArray *fragmentProgram)
        {
            deleteProgram(name);

            Program3D *program=mContext->createProgram();
            program->upload(vertexProgram,fragmentProgram);
            programs[name] = program;

            return program;
        }

        /** Deletes the vertex- and fragment-programs of a certain name. */
        void Starling::deleteProgram(std::string name)
        {
            Program3D *program=getProgram(name);
            if (program)
            {
                program->dispose();
                delete programs[name];
            }
        }

        /** Returns the vertex- and fragment-programs registered under a certain name. */
        Program3D *Starling::getProgram(std::string name)
        {
            return static_cast<Program3D>(programs[name]);
        }

        /** Indicates if a set of vertex- and fragment-programs is registered under a certain name. */
        bool Starling::hasProgram(std::string name)
        {
            return name in programs;
        }

        std::map<std::string, void *> Starling::programs()
        {
            return contextData[PROGRAM_DATA_NAME];
        }

        // properties

        /** Indicates if a context is available and non-disposed. */
        bool Starling::contextValid()
        {
            return (mContext && mContext->driverInfo!= "Disposed");
        }

        /** Indicates if this Starling instance is started. */
        bool Starling::isStarted()
        {
            return mStarted;
        }

        /** The default juggler of this instance. Will be advanced once per frame. */
        Juggler *Starling::juggler()
        {
            return mJuggler;
        }

        /** The render context of this instance. */
        Context3D *Starling::context()
        {
            return mContext;
        }

        /** A dictionary that can be used to save custom data related to the current context.
         *  If you need to share data that is bound to a specific stage3D instance
         *  (e.g. textures), use this dictionary instead of creating a static class variable.
         *  The Dictionary is actually bound to the stage3D instance, thus it survives a
         *  context loss. */
        std::map<std::string, void *> Starling::contextData()
        {
            return static_cast<std::map<std::string, void *>>(sContextData[mStage3D]);
        }

        /** Indicates if multitouch simulation with "Shift" and "Ctrl"/"Cmd"-keys is enabled.
         *  @default false */
        bool Starling::simulateMultitouch()
        {
            return mSimulateMultitouch;
        }
        void Starling::simulateMultitouch(bool value)
        {
            mSimulateMultitouch = value;
            if (mContext) mTouchProcessor->simulateMultitouch= value;
        }

        /** Indicates if Stage3D render methods will report errors. Activate only when needed,
         *  as this has a negative impact on performance. @default false */
        bool Starling::enableErrorChecking()
        {
            return mEnableErrorChecking;
        }
        void Starling::enableErrorChecking(bool value)
        {
            mEnableErrorChecking = value;
            if (mContext) mContext->enableErrorChecking= value;
        }

        /** The antialiasing level. 0 - no antialasing, 16 - maximum antialiasing. @default 0 */
        int Starling::antiAliasing()
        {
            return mAntiAliasing;
        }
        void Starling::antiAliasing(int value)
        {
            if (mAntiAliasing != value)
            {
                mAntiAliasing = value;
                if (contextValid) updateViewPort(true);
            }
        }

        /** The viewport into which Starling contents will be rendered. */
        Rectangle *Starling::viewPort()
        {
            return mViewPort;
        }
        void Starling::viewPort(Rectangle *value)
        {
            mViewPort = value->clone();
        }

        /** The ratio between viewPort width and stage width. Useful for choosing a different
         *  set of textures depending on the display resolution. */
        float Starling::contentScaleFactor()
        {
            return (mViewPort->width* mNativeStageContentScaleFactor) / mStage->stageWidth;
        }

        /** A Flash Sprite placed directly on top of the Starling content. Use it to display native
         *  Flash components. */
        Sprite *Starling::nativeOverlay()
        {
            return mNativeOverlay;
        }

        /** Indicates if a small statistics box (with FPS, memory usage and draw count) is displayed. */
        bool Starling::showStats()
        {
            return mStatsDisplay && mStatsDisplay->parent;
        }
        void Starling::showStats(bool value)
        {
            if (value == showStats) return;

            if (value)
            {
                if (mStatsDisplay) mStage->addChild(mStatsDisplay);
                else               showStatsAt();
            }
            else mStatsDisplay->removeFromParent();
        }

        /** Displays the statistics box at a certain position. */
        void Starling::showStatsAt(std::string hAlign, std::string vAlign, float scale)
        {
            if (mContext == NULL)
            {
                // Starling is not yet ready - we postpone this until it's initialized.
                addEventListener(starling->events->Event::ROOT_CREATED,onRootCreated);
            }
            else
            {
                if (mStatsDisplay == NULL)
                {
                    mStatsDisplay = new StatsDisplay();
                    mStatsDisplay->touchable= false;
                    mStage->addChild(mStatsDisplay);
                }

                int stageWidth = mStage->stageWidth;
                int stageHeight= mStage->stageHeight;

                mStatsDisplay->scaleX= mStatsDisplay->scaleY= scale;

                if (hAlign == HAlign::LEFT)mStatsDisplay->x= 0;
                else if (hAlign == HAlign::RIGHT)mStatsDisplay->x= stageWidth - mStatsDisplay->width;
                else mStatsDisplay->x= int((stageWidth - mStatsDisplay->width)/ 2);

                if (vAlign == VAlign::TOP)mStatsDisplay->y= 0;
                else if (vAlign == VAlign::BOTTOM)mStatsDisplay->y= stageHeight - mStatsDisplay->height;
                else mStatsDisplay->y= int((stageHeight - mStatsDisplay->height)/ 2);
            }

            void Starling::()
            {
                showStatsAt(hAlign, vAlign, scale);
                removeEventListener(starling->events->Event::ROOT_CREATED,onRootCreated);
            }
        }

        /** The Starling stage object, which is the root of the display tree that is rendered. */
        Stage *Starling::stage()
        {
            return mStage;
        }

        /** The Flash Stage3D object Starling renders into. */
        Stage3D *Starling::stage3D()
        {
            return mStage3D;
        }

        /** The Flash (2D) stage object Starling renders beneath. */
        flash::display::Stage *Starling::nativeStage()
        {
            return mNativeStage;
        }

        /** The instance of the root class provided in the constructor. Available as soon as
         *  the event 'ROOT_CREATED' has been dispatched. */
        DisplayObject *Starling::root()
        {
            return mRoot;
        }

        /** Indicates if the Context3D render calls are managed externally to Starling,
         *  to allow other frameworks to share the Stage3D instance. @default false */
        bool Starling::shareContext()
        {
            return mShareContext;
        }
        void Starling::shareContext(bool value)
        {
            mShareContext = value;
        }

        /** The Context3D profile as requested in the constructor. Beware that if you are
         *  using a shared context, this might not be accurate. */
        std::string Starling::profile()
        {
            return mProfile;
        }

        /** Indicates that if the device supports HiDPI screens Starling will attempt to allocate
         *  a larger back buffer than indicated via the viewPort size. Note that this is used
         *  on Desktop only; mobile AIR apps still use the "requestedDisplayResolution" parameter
         *  the application descriptor XML. */
        bool Starling::supportHighResolutions()
        {
            return mSupportHighResolutions;
        }
        void Starling::supportHighResolutions(bool value)
        {
            if (mSupportHighResolutions != value)
            {
                mSupportHighResolutions = value;
                if (contextValid) updateViewPort(true);
            }
        }

        // static properties

        /** The currently active Starling instance. */
        Starling *Starling::current()
        {
            return sCurrent;
        }

        /** The render context of the currently active Starling instance. */
        Context3D *Starling::context()
        {
            return sCurrent ? sCurrent->context: NULL;
        }

        /** The default juggler of the currently active Starling instance. */
        Juggler *Starling::juggler()
        {
            return sCurrent ? sCurrent->juggler: NULL;
        }

        /** The contentScaleFactor of the currently active Starling instance. */
        float Starling::contentScaleFactor()
        {
            return sCurrent ? sCurrent->contentScaleFactor: 1.0;
        }

        /** Indicates if multitouch input should be supported. */
        bool Starling::multitouchEnabled()
        {
            return Multitouch::inputMode== MultitouchInputMode::TOUCH_POINT;
        }

        void Starling::multitouchEnabled(bool value)
        {
            if (sCurrent) throw new IllegalOperationError(
                    "'multitouchEnabled' must be set before Starling instance is created");
            else
                Multitouch::inputMode= value ? MultitouchInputMode::TOUCH_POINT:
                                       MultitouchInputMode::NONE;
        }

        /** Indicates if Starling should automatically recover from a lost device context.
         *  On some systems, an upcoming screensaver or entering sleep mode may
         *  invalidate the render context. This setting indicates if Starling should recover from
         *  such incidents. Beware that this has a huge impact on memory consumption!
         *  It is recommended to enable this setting on Android and Windows, but to deactivate it
         *  on iOS and Mac OS X. @default false */
        bool Starling::handleLostContext()
        {
            return sHandleLostContext;
        }
        void Starling::handleLostContext(bool value)
        {
            if (sCurrent) throw new IllegalOperationError(
                    "'handleLostContext' must be set before Starling instance is created");
            else
                sHandleLostContext = value;
        }
    }
}

