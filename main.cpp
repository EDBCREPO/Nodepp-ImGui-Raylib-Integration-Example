/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <nodepp/encoder.h>
#include <nodepp/json.h>
#include <nodepp/http.h>

#include "imgui/imgui.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace RL {
    #include "imgui/raylib.h"
    #include "imgui/raymath.h"
    #include "imgui/rlImGui.h"
}

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

class imWindow_t { public:

    using NODE_CLB = function_t<int>;

    struct NODE {
        NODE_CLB node_clb;
        string_t name;
        bool state;
    };  ptr_t<NODE> obj;

    imWindow_t( string_t name, NODE_CLB node_clb ) : obj( new NODE() ){
        obj->state = true; obj->node_clb = node_clb; obj->name = name;
    }

    imWindow_t() : obj( new NODE() ) { obj->state=false; }

    void close() const noexcept { obj->state = false; }

    int emit() const noexcept {
        if( obj->state == 0 ){ return -1; }
	ImGui::Begin( obj->name.get(), &obj->state );
        if( obj->node_clb() ==-1 ){ close(); }
    ImGui::End();
    return 1; }

};

/*────────────────────────────────────────────────────────────────────────────*/

queue_t<imWindow_t> list;

/*────────────────────────────────────────────────────────────────────────────*/

void appendWindow() { auto id = encoder::key::generate(5);
    list.push( imWindow_t( id, [=](){

        if( ImGui::Button( "hello world" ) )
          { console::log("hello world Im",id ); }

        return 1;
    }));
}

/*────────────────────────────────────────────────────────────────────────────*/

int main( int argc, char** argv ) {

    /*─······································································─*/

    RL::SetConfigFlags( RL::FLAG_WINDOW_RESIZABLE );
    RL::InitWindow( 800, 600, "raylib - imgui"); 
    RL::SetTargetFPS( 60 );
    RL::rlImGuiSetup(true);

    /*─······································································─*/

    ptr_t<string_t> api_response = new string_t("hello world!");

    /*─······································································─*/

    list.push( imWindow_t( "window_1", [=](){
        static float f = 0.0f;

        for( auto x: regex::match_all( *api_response, "[^\n]+" ) ){
             ImGui::Text( "%s", x.get() );
        }

        return 1;
    }));

    /*─······································································─*/

    list.push( imWindow_t( "window_2", [=](){
        static float   f = 0.0f;
        static string_t bff ( 1024, '\0' );

			ImGui::Text( "Hello, world!" );
        
        if( ImGui::Button("Close Window") )
          { console::log("closed"); return -1; }
        
        if( ImGui::Button("Create new window ") )
          { appendWindow(); }

        if( ImGui::InputText("string", bff.get(), bff.size() ) )
          { console::log( bff ); }

        if( ImGui::SliderFloat("float", &f, 0.0f, 1.0f) )
          { console::log( f ); }
		
        return 1;
    }));

    /*─······································································─*/

    list.push( imWindow_t( "window_3", [=](){
        static queue_t<string_t> item;
        static string_t bff ( 1024, '\0' );

        ImGui::InputText( "input", bff.get(), bff.size() );

        if( ImGui::Button("save") ) { do {
        if( bff[0] == '\0' ){ break; }
            item.push( bff.copy() );
            memset( bff.get(), 0, bff.size() );
        } while(0); }

        item.map([&]( string_t data ){
            ImGui::Text( "%s", data.get() );
        });
		
        return 1;
    }));

    /*─······································································─*/

    process::add( coroutine::add( COROUTINE(){
    coBegin

        do{ auto args   = fetch_t();
            args.method = "GET";
            args.url    = "http://ip-api.com/json";
            
            args.headers= header_t({ 
                { "Host", url::host( args.url ) }
            });

            http::fetch( args )

            .then([=]( http_t cli ){ 
                auto data = json::parse( cli.read() );
                *api_response  = string::format( "city   : %s \n", data["city"].as<string_t>().get() );
                *api_response += string::format( "query  : %s \n", data["query"].as<string_t>().get() );
                *api_response += string::format( "region : %s \n", data["region"].as<string_t>().get() );
                *api_response += string::format( "country: %s \n", data["country"].as<string_t>().get() );
            })

            .fail([=]( except_t err ){
                *api_response = "something went wrong";
            });

        } while(0); coDelay( 10000 );

    coFinish
    }));

    /*─······································································─*/

    process::add( coroutine::add( COROUTINE(){
    coBegin
 
        RL::BeginDrawing(); 
        RL::ClearBackground( RL::RAYWHITE );
        RL::rlImGuiBegin();

        do { /*-------------*/ auto x=list.first();
        while( x != nullptr ){ auto y=x->next;
           if( x->data.emit()==-1 ){ list.erase(x); }
        x=y; }} while(0);

        RL::rlImGuiEnd(); 
        RL::EndDrawing();
 
        coGoto(0); coFinish
    }));

    /*─······································································─*/

    while( !RL::WindowShouldClose() ) 
         { process::next(); }

    /*─······································································─*/

    RL::rlImGuiShutdown();
    RL::CloseWindow();
    process::exit(1);

    /*─······································································─*/

    return 0;
}

/*────────────────────────────────────────────────────────────────────────────*/