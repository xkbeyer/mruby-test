// mruby-test.cpp : Defines the entry point for the console application.
// This file contains code on playing around w/ the mruby vm.
// Written by Klaus Beyer, 2015
// License MIT
// mruby version 1.1.0
//

#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/class.h>

// Checks the return state, if it has an ruby exception print the information.
bool check_retcode( mrb_state* mrb)
{
    if( mrb->exc ) {
        // Exception
        mrb_value obj = mrb_funcall( mrb, mrb_obj_value( mrb->exc ), "inspect", 0 );
        std::string obj_str( RSTRING_PTR( obj ), RSTRING_LEN( obj ) );
        std::cout << obj_str << std::endl;
        return false;
    }
    return true;
}

// Prints the object or if an exception is present its information.
void eval_retcode( mrb_state* mrb, mrb_value rc )
{
    if( mrb->exc ) {
        // Exception
        mrb_value obj = mrb_funcall( mrb, mrb_obj_value( mrb->exc ), "inspect", 0 );
        std::string obj_str( RSTRING_PTR( obj ), RSTRING_LEN( obj ) );
        std::cout << obj_str << std::endl;
    } else {
        mrb_value obj = mrb_funcall( mrb, rc, "inspect", 0 );
        std::string obj_str( RSTRING_PTR( obj ), RSTRING_LEN( obj ) );
        std::cout << obj_str << std::endl;
    }

}

// A simple prints string.
mrb_value mrb_t_printstr( mrb_state* mrb, mrb_value self )
{
    mrb_value obj;

    mrb_get_args( mrb, "o", &obj );
    if( mrb_string_p( obj ) ) {
        std::string obj_str( RSTRING_PTR( obj ), RSTRING_LEN( obj ) );
        std::cout << "C++: " << obj_str << "\n";
    }

    return obj;
}

// returns a string (in an ruby object) of something like a version information.
mrb_value mrb_host_ver( mrb_state* mrb, mrb_value self )
{
    std::stringstream ver;
    time_t rawtime;

    time( &rawtime );
    ver << "Host Application 0.0.1 build on " << ctime( &rawtime ) << " w/ MSC ver " << _MSC_FULL_VER << " Platform x64\n";
    mrb_value obj = mrb_str_new( mrb, ver.str().c_str(), ver.str().size() );
    return obj;
}


int _tmain(int argc, _TCHAR* argv[])
{

    mrb_state* mrb;
    mrb = mrb_open();
    if ( mrb == nullptr )
    {
        std::cout << "Embedded ruby interpretor init failed\n";
    }

    // Create a function which is globally accessible
    struct RClass *krn = mrb->kernel_module;
    mrb_define_method( mrb, krn, "t_printstr", mrb_t_printstr, MRB_ARGS_REQ( 1 ) );
    mrb_define_method( mrb, krn, "host_ver", mrb_host_ver, MRB_ARGS_REQ( 0 ) );

    // Run the defined function in a string script.
    mrb_load_string(mrb, "t_printstr 'Test coming from mruby\n'");
    mrb_load_string( mrb, "p 'Test pure mruby print statement'" );
    mrb_load_string( mrb, "p host_ver" );

    // Read and executes the file.
    FILE* f = fopen( "./test.rb", "r" );
    mrb_value ret = mrb_load_file( mrb, f);
    fclose( f );
    check_retcode( mrb);

    // Call host_ver the call back to the C++ app, which resides on top level.
    ret = mrb_funcall( mrb, mrb_top_self( mrb ), "host_ver", 0 );
    if( check_retcode( mrb ) ) {
        // Now get the returned object and print it.
        eval_retcode( mrb, ret );
    }

    // Call foo(), which resides on top level.
    ret = mrb_funcall( mrb, mrb_top_self(mrb), "foo", 0 );
    check_retcode( mrb);

    // Call bar('Hallo'), which resides on top level.
    mrb_value strarg = mrb_str_new( mrb, "Hallo", sizeof( "Hallo" ) );
    ret = mrb_funcall( mrb, mrb_top_self( mrb ), "bar", 1, strarg );
    check_retcode( mrb);

    // Call bar(25), which resides on top level.
    mrb_value intarg = mrb_fixnum_value( 25 );
    ret = mrb_funcall( mrb, mrb_top_self( mrb ), "bar", 1, intarg );
    check_retcode( mrb);

    // Call class method
    struct RClass* klasse = mrb_class_get( mrb, "Klasse" );
    mrb_value kls = mrb_obj_value( klasse );
    ret = mrb_funcall( mrb, kls, "bar", 0 );
    check_retcode( mrb );


    // Instantiate class Klasse and call some of the methods.
    mrb_value kls_inst = mrb_class_new_instance( mrb, 0, nullptr, klasse );
    // Call method get_desc().
    ret = mrb_funcall( mrb, kls_inst, "get_desc", 0 );
    if( check_retcode( mrb ) ) {
        // Since the get_desc returns the member @desc, show it:
        eval_retcode( mrb, ret );
    }
    // Call method print, which calls the C++ function mrb_t_printstr
    ret = mrb_funcall( mrb, kls_inst, "print", 0 );
    check_retcode( mrb );

    mrb_close( mrb );
    
    int i;
    std::cin >> i;

    return 0;
}

