#!/usr/bin/perl
#
# Convert OpenGL header files to Duktape C function wrappers 
#

use strict;
use warnings;
use Data::Dumper;

my $VERSION        = "0.1";
my $SCOPE_PREFIX   = "DUK_GL_";
my $DEFAULT_SCOPE  = $SCOPE_PREFIX."OPENGL_1_1";
my $current_scope  = $DEFAULT_SCOPE; 
my $DUKTAPE_INDENT = "\t";

my %OPENGL_TYPEDEF_AS_DUK_PUSH_TYPE = (
    "GLenum"     => "uint",
    "GLboolean"  => "boolean",
    "GLbitfield" => "uint",
    "GLint"      => "int",
    "GLint64"    => "int",
    "GLint64EXT" => "int",
    "GLsizei"    => "int",
    "GLshort"    => "int",
    "GLbyte"     => "int",
    "GLfixed"    => "int",
    "GLubyte"    => "uint",
    "GLushort"   => "uint",
    "GLuint"     => "uint",
    "GLuint64"   => "uint",
    "GLuint64EXT"=> "uint",
    "GLfloat"    => "number",
    "GLclampf"   => "number",
    "GLdouble"   => "number",
    "GLclampd"   => "number",
    "GLhalfNV"   => "uint",
    "const GLchar * const *" => "string", 
    "const GLchar *" => "string",
    "const GLubyte *" => "string",
    "const GLbyte *" => "string",
    "const GLfloat *" => "DETERMINE"
);

my %OPENGL_TYPEDEF_AS_DUK_PUSH_PREDEFINE = (
    "GLDEBUGPROCARB" => "SKIP",
    "GLDEBUGPROC" => "SKIP",
    "GLDEBUGPROCAMD" => "SKIP",
    "GLprogramcallbackMESA" => "SKIP",
    "const GLfloat"  => "SKIP",
    "GLfloat *" => "SKIP",
    "GLdouble *" => "SKIP",
    "GLubyte *" => "SKIP",
    "GLushort *" => "SKIP",
    "GLuint *" => "SKIP",
    "GLint *" => "SKIP",
    "GLint64 *" => "SKIP",
    "GLint64EXT *" => "SKIP",
    "GLuint64 *" => "SKIP",
    "GLuint64EXT *" => "SKIP",
    "GLchar *" => "SKIP",
    "GLenum *" => "SKIP",
    "GLboolean *" => "SKIP",
    "GLsizei *" => "SKIP",
    "void * *" => "SKIP",
    "void *" => "SKIP",
    "GLfixed *" => "SKIP",
    "const GLhalfNV *" => "SKIP",
    "const GLint *" => "SKIP",
    "const GLfixed *" => "SKIP",
    "const GLint64 *" => "SKIP",
    "const GLint64EXT *" => "SKIP",
    "const GLuint64 *" => "SKIP",
    "const GLuint64EXT *" => "SKIP",
    "const GLenum *" => "SKIP",
    "const GLuint *" => "SKIP",
    "const GLushort *" => "SKIP",
    "const GLshort *" => "SKIP",
    "const GLclampf *" => "SKIP",
    "const GLdouble *" => "SKIP",
    "const GLboolean *" => "SKIP",
    "const GLboolean * *" => "SKIP",
    "const GLsizei *" => "SKIP",
    "const void *" => "SKIP",
    "const void * const *" => "SKIP",
    "const void * *" => "SKIP"
);

my $OPENGL_FUNCTION_BIND_MACRO_NAME = "duk_gl_bind_opengl_wrapper";
my $OPENGL_CONSTANT_SET_MACRO_NAME  = "duk_gl_push_opengl_constant_property";

my $use_stdout = 0;
my $input_files = "";
my $output_h_file = "";
my $output_c_file = "";
my $max_function_argument_count = 0;
my %opengl_constant_map = ();
my %opengl_function_map = ();
my %opengl_scope_map    = ();

my $CMD_ARG_HELP       = "--help";
my $CMD_ARG_USAGE      = "--usage";
my $CMD_ARG_VERSION    = "--version";
my $CMD_ARG_INPUT      = "--input";
my $CMD_ARG_OUTPUT_H   = "--output-h";
my $CMD_ARG_OUTPUT_C   = "--output-c";
my $CMD_ARG_STDOUT     = "--stdout";

sub print_version
{
    print "Version: $VERSION\n";
}

sub print_usage
{
    print "Usage: $0 $CMD_ARG_INPUT \"gl.h glext.h\" $CMD_ARG_OUTPUT_H duktape_opengl.h $CMD_ARG_OUTPUT_C duktape_opengl.c\n";
}

sub print_help
{
    print_version();
    print "\n";

    print_usage();
    print "\n";

    print "Commands:\n";
    print "$CMD_ARG_HELP                            - Prints help\n";
    print "$CMD_ARG_USAGE                           - Prints usage\n";
    print "$CMD_ARG_VERSION                         - Prints version\n";
    print "$CMD_ARG_INPUT <opengl header>           - OpenGL header that needs to be processed\n";
    print "$CMD_ARG_OUTPUT_H <GL wrapper header>    - Duktape OpenGL wrapper .h output file\n";
    print "$CMD_ARG_OUTPUT_C <GL wrapper source>    - Duktape OpenGL wrapper .c output file\n";
    print "$CMD_ARG_STDOUT                          - Output as STDOUT\n";
}

sub process_arguments
{
    # check command line arguments 
    for (my $i = 0; $i <= $#ARGV; $i++)
    {
        my $argument = $ARGV[$i];
        if ($argument eq $CMD_ARG_HELP)
        {
            print_help();
            exit();
        }
        elsif ($argument eq $CMD_ARG_USAGE)
        {
            print_usage();
            exit();
        }
        elsif ($argument eq $CMD_ARG_VERSION)
        {
            print_version();
            exit();
        }
        elsif ($argument eq $CMD_ARG_STDOUT)
        {
            $use_stdout = 1;
        }
        elsif ($argument eq $CMD_ARG_INPUT)
        {
            $i++;
            $input_files = $ARGV[$i];
        }
        elsif ($argument eq $CMD_ARG_OUTPUT_H)
        {
            $i++;
            $output_h_file = $ARGV[$i];
        }
        elsif ($argument eq $CMD_ARG_OUTPUT_C)
        {
            $i++;
            $output_c_file = $ARGV[$i];
        }
        else
        {
            print "ERROR: Invalid argument! argument:'$argument'\n\n";
            print_help();
            exit(1);
        }
    }    
}

sub validate_arguments()
{
    if ($#ARGV < 1)
    {
        print_help();
        exit();
    }

    my @file_list = split(" ", $input_files);
    foreach my $input_file (@file_list)
    {
        if (!-f $input_file)
        {
            print "ERROR: $CMD_ARG_INPUT requires existing file(s) as the parameter! invalid_argument:'$CMD_ARG_INPUT $input_files', invalid_file:'$input_file'\n\n";
            print_usage();
            exit(1);
        }
    }
    
    if ($output_h_file eq ""
        && $output_c_file eq ""
        && !$use_stdout)
    {
        print "ERROR: Invalid arguments! One of the following must be provided $CMD_ARG_STDOUT, $CMD_ARG_OUTPUT_H or $CMD_ARG_OUTPUT_C\n\n";
        print_usage();
        exit(1);
    }
}

sub handle_opengl_constant
{
    my $opengl_constant_name  = $_[0];
    my $opengl_constant_value = $_[1];

    die("Constant name not defined!") if $opengl_constant_name eq "";
    die("Constant value not defined!") if $opengl_constant_value eq "";

    #Not standard constants will have different scope which is not enabled by default
    my $previous_scope = $current_scope; 
    if ($opengl_constant_name =~ m/_(ARB|ATI|EXT|HP|IBM|KTX|INTEL|NV|MESA|SGI|SGIX|SUN|WIN)$/)
    {
        $current_scope = $SCOPE_PREFIX . $1;
    }

    #print $handle "Constant found! '$opengl_constant_name' = '$opengl_constant_value'\n";
    my %constant_hash = (
           "name" => $opengl_constant_name,
           "value" => $opengl_constant_value
    );

    $opengl_constant_map{$current_scope."/".$opengl_constant_name} = \%constant_hash;
    
    $opengl_scope_map{$current_scope} = 1;
    
    $current_scope = $previous_scope;
}

sub handle_opengl_function
{
    my $opengl_function_return_type  = $_[0];
    my $opengl_function_name         = $_[1];
    my $opengl_function_arguments    = $_[2];

    die("Function return type not defined!") if $opengl_function_return_type eq "";
    die("Function name not defined!") if $opengl_function_name eq "";
    die("Function arguments not defined!") if $opengl_function_arguments eq "";
    
    #remove heading and tailing whitespaces
    $opengl_function_arguments =~ s/^\s+|\s+$//g;
    #remove whitespaces between comma separators
    $opengl_function_arguments =~ s/\s*,\s*/,/g;
    #convert multiple whitespaces into single space
    $opengl_function_arguments =~ s/\s+/ /g;

    #TODO: take out-of-scope functions to scope at some point  
    #OpenGL functions that have pointer or constant arguments are out of scope
    #if ($opengl_function_arguments =~ m/\*|const/) { return; }
    
    
    #pointer returns and arguments are out of scope
    if ($opengl_function_return_type =~ m/GL\w*(ptr|handle|vdpau|sync)/
        || $opengl_function_arguments =~ m/GL\w*(ptr|handle|vdpau|sync)/) { return 0; }

    #Not standard functions will have different scope which is not enabled by default    
    my $previous_scope = $current_scope; 
    if ($opengl_function_name =~ m/(ARB|ATI|EXT|HP|IBM|KTX|INTEL|NV|MESA|SGI|SGIX|SUN|WIN)$/)
    {
        $current_scope = $SCOPE_PREFIX . $1;
    }
    
    #print "Function found! $opengl_function_return_type $opengl_function_name (\n";
    
    $opengl_function_arguments =~ s/\*/ * /g;
    $opengl_function_arguments =~ s/GLvoid/void/g;
    #strip whitespaces
    $opengl_function_arguments =~ s/\s+/ /g;
    $opengl_function_arguments =~ s/\s+$//g;
    $opengl_function_arguments =~ s/^\s+//g;
    $opengl_function_arguments =~ s/\s*,\s*/,/g;
    #strip argument variable names, we're only interested about variable types
    $opengl_function_arguments =~ s/\s*[\w\d]+\s*(,|$)/,/g;

    my @opengl_function_argument_array = split(",", $opengl_function_arguments);
    for my $argument (@opengl_function_argument_array)
    {
        my $predefine = $OPENGL_TYPEDEF_AS_DUK_PUSH_PREDEFINE{$argument};
        if (defined $predefine)
        {
            if ($predefine eq "SKIP")
            {
                return 0;
            }
        }
        
        my $duk_push_type = $OPENGL_TYPEDEF_AS_DUK_PUSH_TYPE{$argument};
        die "Could not convert argument type to duk push type! function:'$opengl_function_name', argument_type:'$argument'" if $duk_push_type eq "";
    }
    
    if ($max_function_argument_count < $#opengl_function_argument_array)
    {
        $max_function_argument_count = $#opengl_function_argument_array;
    }

    my %function_hash = (
        "return_type" => $opengl_function_return_type,
        "name" => $opengl_function_name,
        "arguments" => \@opengl_function_argument_array
    );

    $opengl_function_map{$current_scope."/".$opengl_function_name} = \%function_hash;
    
    $opengl_scope_map{$current_scope} = 1;

    $current_scope = $previous_scope;
    #print(");\n");
    
    return 1;
}

sub process_input_files()
{
    my $is_multiple_line_function_definition = 0;
    
    my $opengl_function_return_type   = "";
    my $opengl_function_name          = "";
    my $opengl_function_arguments     = "";
    
    my @file_list = split(" ", $input_files);
    foreach my $input_file (@file_list)
    {
        open(HEADER_FILE, '<', $input_file);
        while(<HEADER_FILE>)
        {
            chomp;
            
            #process white spaces
            $_ =~ s/\s+/ /g;
            
            #match OpenGL constant definitions
            #example: #define GL_LINE_LOOP   0x0002
            if ($_ =~ m/^#define (W?GL[A-Z0-9_]+)\s+((0x)?[0-9A-F]+).*$/m)
            {
                if ($is_multiple_line_function_definition)
                {
                    die("Parsing error while parsing multiline function! function:'$opengl_function_return_type $opengl_function_name'");
                }
    
                my $opengl_constant_name  = $1;
                my $opengl_constant_value = $2;
                
                handle_opengl_constant($opengl_constant_name, $opengl_constant_value);
            }
           #GLAPI void APIENTRY glMultiTexCoord3s (GLenum target, GLshort s, GLshort t, GLshort r); 
            #match OpenGL single line function definitions
            #example: GLAPI void GLAPIENTRY glVertex2f( GLfloat x, GLfloat y );
            elsif ($_ =~ m/^GLAPI\s+(\w+)\s+G?L?APIENTRY\s+([\w\d]+)\s*\(\s*([\w\d\s\*,_]+)\s*\)\s*;.*$/m)
            {
                if ($is_multiple_line_function_definition)
                {
                    die("Parsing error while parsing multiline function! function:'$opengl_function_return_type $opengl_function_name'");
                }
    
                $opengl_function_return_type   = $1;
                $opengl_function_name          = $2;
                $opengl_function_arguments     = $3;
                
                if (!handle_opengl_function($opengl_function_return_type, $opengl_function_name, $opengl_function_arguments))
                {
                    print STDOUT "* $opengl_function_return_type $opengl_function_name ($opengl_function_arguments)\n";
                }
            }
            
            #match OpenGL multiline function definitions
            #example:
            #GLAPI void GLAPIENTRY glOrtho( GLdouble left, GLdouble right,
            #                         GLdouble bottom, GLdouble top,
            #                         GLdouble near_val, GLdouble far_val );
            elsif ($_ =~ m/^GLAPI\s+(\w+)\s+GLAPIENTRY\s+([\w\d]+)\s*\(\s*([\w\d\s\*,_]+)\s*$/m)
            {
                if ($is_multiple_line_function_definition)
                {
                    die("Parsing error while parsing multiline function! function:'$opengl_function_return_type $opengl_function_name'");
                }
    
                $opengl_function_return_type   = $1;
                $opengl_function_name          = $2;
                $opengl_function_arguments     = $3;
    
                $is_multiple_line_function_definition = 1;
            }
            elsif ($is_multiple_line_function_definition && $_ =~ m/^\s+([\w\d\s\*,_]+)\s*$/m)
            {
                $opengl_function_arguments .= $1;
            }
            elsif ($is_multiple_line_function_definition && $_ =~ m/^\s+([\w\d\s\*,_]+)\s*\)\s*;.*$/m)
            {
                $opengl_function_arguments .= $1;
    
                if (!handle_opengl_function($opengl_function_return_type, $opengl_function_name, $opengl_function_arguments))
                {
                    print STDOUT "* $opengl_function_return_type $opengl_function_name ($opengl_function_arguments)\n";
                }

                $is_multiple_line_function_definition = 0;
            }
            
            #match partial comment that describes to which version functionality belongs to
            #Example: * OpenGL 1.2
            elsif ($_ =~ m/^\s+\*\s+(OpenGL [\d\.]+)/m)
            {
                $current_scope = $SCOPE_PREFIX . uc($1);
                $current_scope =~ s/ |\./_/g;
            }
            #match partial comment that describes to which version functionality belongs to
            #Example: * OpenGL 1.2
            elsif ($_ =~ m/^#ifndef\s+GL_VERSION_(\d+_\d+)/m)
            {
                $current_scope = $SCOPE_PREFIX . "OPENGL_" . $1;

                #OpenGL 1.0 and 1.1 shall be in the same scope (1.1)
                if ($current_scope eq $SCOPE_PREFIX . "OPENGL_1_0")
                {
                    $current_scope = $DEFAULT_SCOPE;
                }
            }
        }
    }
}

sub validate_processed_data()
{
    die "Constant map uninitialized!" if keys %opengl_constant_map eq 0;
    die "Function map uninitialized!" if keys %opengl_function_map eq 0;
}

#get C style comment in Duktape style
sub get_c_comment
{
    my $comment = $_[0];
    die "No comment defined!" if $comment eq "";

    my $c_style_comment = "";
    
    my @comment_lines = split("\n", $comment);
    
    $c_style_comment .= "/*\n";
    foreach my $comment_line (@comment_lines)
    {
        $c_style_comment .= " *  $comment_line\n";
    }
    $c_style_comment .= " */\n";
    
    return $c_style_comment;
}

sub get_boilerplate_c_comment()
{
    return get_c_comment(
        "Duktape OpenGL $VERSION - OpenGL bindings for Duktape embeddable Javascript engine.\n"
       ."See AUTHORS.rst and LICENSE.txt for copyright and licensing information.\n"
       ."\n"
       ."Duktape OpenGL: https://github.com/mrautio/duktape-opengl/\n"
       ."Duktape: http://duktape.org/\n"
       ."OpenGL API reference: https://www.opengl.org/registry/\n"
       )."\n";
}

#
# .h file generation functions
#
sub output_header_file
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_boilerplate_c_comment();

    print $handle get_c_comment("BEGIN PUBLIC API");
    
    print $handle "#ifndef DUK_GL_API_PUBLIC_H_INCLUDED\n";
    print $handle "#define DUK_GL_API_PUBLIC_H_INCLUDED\n";
    print $handle "\n";
    
    print $handle get_c_comment("Avoid C++ name mangling");
    
    print $handle "\n";
    print $handle "#ifdef __cplusplus\n";
    print $handle "extern \"C\" {\n";
    print $handle "#endif\n";
    
    print $handle "\n";
    print $handle "DUK_EXTERNAL_DECL void duk_gl_push_opengl_bindings(duk_context *ctx);\n";
    print $handle "DUK_EXTERNAL_DECL void duk_gl_bind_opengl_functions(duk_context *ctx);\n";
    print $handle "DUK_EXTERNAL_DECL void duk_gl_set_constants(duk_context *ctx);\n";
    print $handle "\n";
    
    print $handle get_c_comment("C++ name mangling");
    
    print $handle "\n";
    print $handle "#ifdef __cplusplus\n";
    print $handle "/* end 'extern \"C\"' wrapper */\n";
    print $handle "}\n";
    print $handle "#endif\n";
    
    print $handle "\n";
    print $handle "#endif  /* DUK_GL_API_PUBLIC_H_INCLUDED */\n";
    print $handle "\n";
}

#
# .c file generation functions
#

sub output_c_file_header
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_boilerplate_c_comment();
    
    print $handle "#include <duktape.h>\n";
    print $handle "\n";
    print $handle "#ifdef __APPLE__\n";
    my @file_list = split(" ", $input_files);
    foreach my $input_file (@file_list)
    {
        print $handle "#include <OpenGL/$input_file>\n";
    }
    print $handle "#else\n";
    foreach my $input_file (@file_list)
    {
        print $handle "#include <GL/$input_file>\n";
    }
    print $handle "#endif\n";

    print $handle "\n";
    print $handle "#define $DEFAULT_SCOPE\n\n";
    print $handle get_c_comment("Bindings that are not enabled by default.");
    my $is_opengl_version_scope_handling = 0;
    foreach my $scope ( sort keys %opengl_scope_map )
    {
        if ($scope =~ m/OPENGL/)
        {
            if ($scope eq $DEFAULT_SCOPE)
            {
                $is_opengl_version_scope_handling = 1;

                print $handle "\n";
                print $handle get_c_comment("Enable automatically older OpenGL standard manjor versions, if higher major version is enabled");
                print $handle "#ifdef ".$SCOPE_PREFIX."OPENGL_4X\n";
                print $handle "#define ".$SCOPE_PREFIX."OPENGL_3X\n";
                print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_4X*/\n\n";
                
                print $handle "#ifdef ".$SCOPE_PREFIX."OPENGL_3X\n";
                print $handle "#define ".$SCOPE_PREFIX."OPENGL_2X\n";
                print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_3X*/\n\n";
                
                print $handle "#ifdef ".$SCOPE_PREFIX."OPENGL_2X\n";
                print $handle "#define ".$SCOPE_PREFIX."OPENGL_1X\n";
                print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_2X*/\n\n";
                
                print $handle "#ifdef  ".$SCOPE_PREFIX."OPENGL_1X\n";
            }
            elsif ($scope =~ m/OPENGL_(\d+)_0/)
            {
                print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_".($1-1)."X*/\n\n";
                print $handle "#ifdef  ".$SCOPE_PREFIX."OPENGL_".($1)."X\n";
            }

            print $handle "#define $scope\n";
        }
        else
        {
            if ($is_opengl_version_scope_handling == 1)
            {
                print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_4X*/\n\n";
                $is_opengl_version_scope_handling = 0;
            }

            print $handle "/*#define $scope*/\n";
        }
    }
    
    if ($is_opengl_version_scope_handling == 1)
    {
        print $handle "#endif /*".$SCOPE_PREFIX."OPENGL_4X*/\n";
        $is_opengl_version_scope_handling = 0;
    }

    print $handle "\n";
}

sub output_binding_function_macro
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_c_comment("Macro for binding OpenGL wrapper C function as Duktape JavaScript function");
    print $handle "#define $OPENGL_FUNCTION_BIND_MACRO_NAME(ctx, c_function_name, argument_count) \\\n";
    print $handle $DUKTAPE_INDENT."duk_push_c_function((ctx), duk_gl_##c_function_name, (argument_count)); \\\n";
    print $handle $DUKTAPE_INDENT."duk_put_prop_string((ctx), -2, #c_function_name)\n\n";
}

sub output_property_set_macro
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_c_comment("Macro for setting OpenGL constants");
    print $handle "#define $OPENGL_CONSTANT_SET_MACRO_NAME(ctx, opengl_constant) \\\n";
    print $handle $DUKTAPE_INDENT."duk_push_uint((ctx), (opengl_constant)); \\\n";
    print $handle $DUKTAPE_INDENT."duk_put_prop_string((ctx), -2, #opengl_constant)\n\n";
}


sub output_array_handling_macros
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];
    
    print $handle get_c_comment("Macro for handling of arrays");

    print $handle "DUK_LOCAL size_t duk_gl_determine_array_length(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, size_t num)\n";
    print $handle "{\n";
    print $handle $DUKTAPE_INDENT."size_t array_length = sz;\n";
    print $handle $DUKTAPE_INDENT."if (sz < 1)\n";
    print $handle $DUKTAPE_INDENT."{\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."/* use <arrayVariable>.length is array size not explicitly defined */\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_get_prop_string(ctx, obj_index, \"length\");\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."array_length = (unsigned int)duk_get_uint(ctx, -1);\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_pop(ctx);\n";
    print $handle $DUKTAPE_INDENT."}\n";
    print $handle $DUKTAPE_INDENT."/* prevent buffer overflow by clamping the value */\n";
    print $handle $DUKTAPE_INDENT."if (array_length > num)\n";
    print $handle $DUKTAPE_INDENT."{\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."array_length = num;\n";
    print $handle $DUKTAPE_INDENT."}\n";
    print $handle $DUKTAPE_INDENT."return array_length;\n";
    print $handle "}\n\n";

    print $handle "#define DUK_GL_ARRAY_PROCESSING_FUNCTION(argtypedef1, arg1) \\\n";
    print $handle "DUK_LOCAL argtypedef1 *duk_gl_get_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \\\n";
    print $handle "{ \\\n";
    print $handle $DUKTAPE_INDENT."if (duk_is_array(ctx, obj_index)) \\\n";
    print $handle $DUKTAPE_INDENT."{ \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."size_t array_length = duk_gl_determine_array_length(ctx, obj_index, sz, num); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."unsigned int i = 0; \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."for(i=0; i<array_length; i++) \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."{ \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_get_prop_index(ctx, obj_index, (duk_uarridx_t)i); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT.$DUKTAPE_INDENT."array[i] = (argtypedef1)duk_get_##arg1(ctx, -1); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_pop(ctx); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."} \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."return array; \\\n";
    print $handle $DUKTAPE_INDENT."} \\\n";
    print $handle $DUKTAPE_INDENT."return array; \\\n";
    print $handle "} \\\n";
    print $handle "DUK_LOCAL size_t duk_gl_put_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \\\n";
    print $handle "{ \\\n";
    print $handle $DUKTAPE_INDENT."if (duk_is_array(ctx, obj_index)) \\\n";
    print $handle $DUKTAPE_INDENT."{ \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_get_prop(ctx, obj_index); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."size_t array_length = duk_gl_determine_array_length(ctx, obj_index, sz, num); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."unsigned int i = 0; \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."for(i=0; i<array_length; i++) \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."{ \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_push_##arg1(ctx, (argtypedef1)array[i]); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_put_prop_index(ctx, obj_index, (duk_uarridx_t)i); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."} \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."duk_pop(ctx); \\\n";
    print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."return array_length; \\\n";
    print $handle $DUKTAPE_INDENT."} \\\n";
    print $handle $DUKTAPE_INDENT."return 0; \\\n";
    print $handle "}\n\n";
    
    print $handle "DUK_GL_ARRAY_PROCESSING_FUNCTION(GLbyte, number)\n";
    print $handle "DUK_GL_ARRAY_PROCESSING_FUNCTION(GLdouble, number)\n";
    print $handle "DUK_GL_ARRAY_PROCESSING_FUNCTION(GLfloat, number)\n";
    print $handle "DUK_GL_ARRAY_PROCESSING_FUNCTION(GLint, number)\n";
    print $handle "DUK_GL_ARRAY_PROCESSING_FUNCTION(GLshort, number)\n";
    print $handle "\n";
}

sub get_opengl_function_wrapper_macro_name
{
    my $return_type_count       = $_[0];
    my $function_argument_count = $_[1];

    die("Invalid return type count! count:'$return_type_count'") if $return_type_count eq "" || $return_type_count < 0;
    die("Invalid function argument count! count:'$function_argument_count'") if $function_argument_count eq "" || $function_argument_count < 0;
    
    return "DUK_GL_C_WRAPPER_FUNCTION_RET".$return_type_count."_ARG".$function_argument_count;
}

sub output_wrapper_function_macro
{
    my $handle = $_[0];
    my $return_type_count       = $_[1];
    my $function_argument_count = $_[2];
    
    die("Invalid return type count! count:'$return_type_count'") if $return_type_count eq "" || $return_type_count < 0;
    die("Invalid function argument count! count:'$function_argument_count'") if $function_argument_count eq "" || $function_argument_count < 0;
    die("Only void or single type function returns are supported!") if $return_type_count > 1;

    if ($function_argument_count == 0)
    {
        print $handle "#define ".get_opengl_function_wrapper_macro_name($return_type_count, $function_argument_count)."(PREPROCESS, POSTPROCESS, c_function_name";
        if ($return_type_count == 1)
        {
            print $handle ", rettypedef1";
        }
        print $handle ") \\\n";
        
        print $handle "DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \\\n";
        print $handle "{ \\\n";
        print $handle $DUKTAPE_INDENT."PREPROCESS; \\\n";
        if ($return_type_count == 0)
        {
            print $handle $DUKTAPE_INDENT."c_function_name(); \\\n";
        }
        else
        {
            print $handle $DUKTAPE_INDENT."duk_push_##rettypedef1(ctx, c_function_name()); \\\n";
        }
    }
    else
    {
        print $handle "#define ".get_opengl_function_wrapper_macro_name($return_type_count, $function_argument_count)."(PREPROCESS, POSTPROCESS, c_function_name";
        
        if ($return_type_count == 1)
        {
            print $handle ", rettypedef1";
        }

        for(my $i = 1; $i <= $function_argument_count; $i++)
        {
            print $handle ", argtypedef$i, arg$i";
        }
        
        print $handle ") \\\n";
        
        print $handle "DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \\\n";
        print $handle "{ \\\n";
        print $handle $DUKTAPE_INDENT."PREPROCESS; \\\n";

        if ($return_type_count == 0)
        {
            print $handle $DUKTAPE_INDENT."c_function_name( \\\n";
        }
        else
        {
            print $handle $DUKTAPE_INDENT."duk_push_##rettypedef1(ctx, c_function_name( \\\n";
        }

        for(my $i = 1; $i <= $function_argument_count; $i++)
        {
            print $handle $DUKTAPE_INDENT.$DUKTAPE_INDENT."(argtypedef".$i.")arg".$i;
            if ($i != $function_argument_count)
            {
                print $handle ", ";
            }
            print $handle " \\\n";
        }
        
        print $handle $DUKTAPE_INDENT;
        if ($return_type_count == 1)
        {
            print $handle ")";
        }
        print $handle "); \\\n";
    }
    
    print $handle $DUKTAPE_INDENT."POSTPROCESS; \\\n";
    print $handle $DUKTAPE_INDENT."return $return_type_count; \\\n";
    print $handle "}\n\n";  
}

sub output_wrapper_function_macros
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_c_comment(
       "Wrapper macros for OpenGL C functions.\n"
       ."Macro name defines the amount of return types and function arguments it supports.\n"
       ."Macros take arguments in following manner (<Function name>, <OpenGL variable type 1>, <Duktape push type 1>, ...N)\n"
       ."- Function name = OpenGL C function name\n"
       ."- OpenGL variable type N = OpenGL's C variable type definition, for example, \"GLenum\"\n"
       ."- Duktape push type 1 = Duktape API's duk_get_... function's type name, for example, duk_get_number\n"
    );
    
    for(my $arg_i = 0; $arg_i <= $max_function_argument_count + 1; $arg_i++)
    {
        output_wrapper_function_macro($handle, 0, $arg_i);
        output_wrapper_function_macro($handle, 1, $arg_i);
    }
}

sub output_wrapper_functions
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];

    print $handle get_c_comment("OpenGL wrapper function definitions");
    my $old_scope = "";
    foreach my $key ( sort keys %opengl_function_map )
    {
        my($scope, $constant_name) = split("/", $key);
        
        if ($old_scope ne "" && $old_scope ne $scope)
        {
            print $handle "#endif /* $old_scope */\n\n";
        }
        
        if ($old_scope ne $scope)
        {
            $old_scope = $scope;
            print $handle "#ifdef $scope\n";
        }

        my %function = %{$opengl_function_map{$key}};

        my $return_type_count = 0;
        if ($function{'return_type'} !~ m/(void|GLvoid)/)
        {
            $return_type_count = 1;
        }

        my $macro_preprocess = "";
        my $macro_postprocess = "";
        my $macro_body = "";

        my $is_dynamic_size_definition = -1;
        my $argi = 0;
        foreach my $argument ( @{$function{'arguments'}} )
        {
	        if ($argument =~ m/GLsizei/)
	        {
	            $is_dynamic_size_definition = $argi;
	            my $duk_push_type = $OPENGL_TYPEDEF_AS_DUK_PUSH_TYPE{$argument};
	            die "Could not convert argument type to duk push type! function:'$function{'name'}', argument_type:'$argument', dump:'".Dumper(\%function)."'" if $duk_push_type eq "";

	            $macro_preprocess .= "GLsizei var$argi = duk_get_$duk_push_type(ctx, $argi); "
	        }
	        
	        $argi++;
        }

        $argi = 0;
        foreach ( @{$function{'arguments'}} )
        {
            my $c_function_argument_type = $_;
            
            my $duk_push_type = $OPENGL_TYPEDEF_AS_DUK_PUSH_TYPE{$c_function_argument_type};
                
            die "Could not convert argument type to duk push type! function:'$function{'name'}', argument_type:'$c_function_argument_type', dump:'".Dumper(\%function)."'" if $duk_push_type eq "";
                
            if ($duk_push_type ne "DETERMINE")
            {
            	if ($c_function_argument_type !~ m/GLsizei/)
            	{
            		$macro_body .= ", $c_function_argument_type, duk_get_$duk_push_type(ctx, $argi)";
            	}
            	else
            	{
            	   $macro_body .= ", $c_function_argument_type, var$argi";
            	}
                
            }
            else #arrays etc...
            {
                #Try to find out array size definitions from function names, examples:
                #- glGetnMapfv (float array where size defined synamizally in GLsizei argument variable)
                #- glScissorIndexedv (1 element double array)
                #- glGetVertexArrayIndexed64iv (1 element 64bit integer array)
                #- glVertexAttribL4dv (4 element double array)
                #- glProgramUniformMatrix3x4fv (12 element matrix float  array)
                #- glProgramUniformMatrix4fv (16 element matrix float array)

                my $arraySize = 0;
                if ($function{'name'} =~ m/(Matrix)?((\dx)?\d)??(64i|64ui|f|d|s|us|i|ui|ub)v(ARB|ATI|EXT|HP|IBM|KTX|INTEL|NV|MESA|SGI|SGIX|SUN|WIN)?$/)
                {
                    my $functionDefinition = $1||"";
                    my $arrayType = $4||"";
                    my $arrayNumber1 = 1;
                    my $arrayNumber2 = 1;

                    my $sizeDefinition = $2||"";
                    if ($sizeDefinition =~ m/(\d?)x?(\d?)/ && $sizeDefinition !~ m/\d{2,}/)
                    {
                        $arrayNumber1 = $1||1;
                        $arrayNumber2 = $2||1;
                    }

                    $arraySize = $arrayNumber1*$arrayNumber2;
                    if ($functionDefinition eq "Matrix" && $arrayNumber2 == 1)
                    {
                        $arraySize = $arrayNumber1*$arrayNumber1;
                    }
                    #print STDOUT "Found function! dynamic:'$is_dynamic_size_definition', 1:'$functionDefinition', 2:'$arrayNumber1',3:'$arrayNumber2',4:'$arrayType',arraySize:'$arraySize',name:'$opengl_function_name', args:'$opengl_function_arguments'\n";
                }

                my $variable_name = "var$argi";
                $macro_body .= ", $c_function_argument_type, duk_gl_get_GLfloat_array(ctx, $argi, $arraySize, $variable_name, $arraySize)";

                my $array_type = $c_function_argument_type;
                die "2d pointers are not supported! dump:'".Dumper(\%function)."'" if $array_type =~ m/\* \*/; 

                $array_type =~ s/const//g;
                $array_type =~ s/\*/$variable_name [$arraySize]/g;

                $macro_preprocess .= "$array_type; "
            }
            
            $argi++;
        }

        if ($macro_preprocess eq "")
        {
        	$macro_preprocess = "/*EMPTY*/";
        }
        if ($macro_postprocess eq "")
        {
            $macro_postprocess = "/*EMPTY*/";
        }

        print $handle get_opengl_function_wrapper_macro_name($return_type_count, ($#{$function{'arguments'}} + 1))."($macro_preprocess,$macro_postprocess,$function{'name'}";
        if ($return_type_count == 1)
        {
            my $c_function_argument_type = $function{'return_type'};
            my $duk_push_type = $OPENGL_TYPEDEF_AS_DUK_PUSH_TYPE{$c_function_argument_type};
                
            die "Could not convert return type to duk push type! function:'$function{'name'}', argument_type:'$c_function_argument_type'" if $duk_push_type eq "" || $duk_push_type eq "DETERMINE";

            print $handle ", $duk_push_type";
        }
        print $handle $macro_body;
        print $handle ")\n";

    }
    if ($old_scope ne "")
    {
        print $handle "#endif /* $old_scope */\n\n";
    }
}

sub output_initialization_function
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];
    
    print $handle get_c_comment("Push OpenGL bindings to JavaScript global object");
    print $handle "void duk_gl_push_opengl_bindings(duk_context *ctx)\n";
    print $handle "{\n";
    print $handle $DUKTAPE_INDENT."duk_push_global_object(ctx);\n";
    print $handle $DUKTAPE_INDENT."duk_gl_bind_opengl_functions(ctx);\n";
    print $handle $DUKTAPE_INDENT."duk_gl_set_constants(ctx);\n";
    print $handle $DUKTAPE_INDENT."duk_pop(ctx);\n";
    print $handle "}\n\n";
}

sub output_function_bindings
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];
    
    print $handle get_c_comment("OpenGL function bindings to JavaScript");
    print $handle "void duk_gl_bind_opengl_functions(duk_context *ctx)\n";
    print $handle "{\n";
    
    my $old_scope = "";
    foreach my $key ( sort keys %opengl_function_map )
    {
        my($scope, $constant_name) = split("/", $key);
        
        if ($old_scope ne "" && $old_scope ne $scope)
        {
            print $handle "#endif /* $old_scope */\n\n";
        }
        
        if ($old_scope ne $scope)
        {
            $old_scope = $scope;
            print $handle "#ifdef $scope\n";
        }

        my %function = %{$opengl_function_map{$key}};
            
        print $handle $DUKTAPE_INDENT."$OPENGL_FUNCTION_BIND_MACRO_NAME(ctx, $function{'name'}, ".($#{$function{'arguments'}} + 1).");\n";
    }
    if ($old_scope ne "")
    {
        print $handle "#endif /* $old_scope */\n\n";
    }
    
    print $handle "}\n\n";
}

sub output_constants
{
    die "Expecting file handle as the argument!" if scalar(@_) != 1;
    my $handle = $_[0];
    
    print $handle get_c_comment("OpenGL constants to JavaScript");
    print $handle "void duk_gl_set_constants(duk_context *ctx)\n";
    print $handle "{\n";
    
    my $old_scope = "";
    foreach my $key ( sort keys %opengl_constant_map )
    {
        my($scope, $constant_name) = split("/", $key);
        
        if ($old_scope ne "" && $old_scope ne $scope)
        {
            print $handle "#endif /* $old_scope */\n\n";
        }
        
        if ($old_scope ne $scope)
        {
            $old_scope = $scope;
            print $handle "#ifdef $scope\n";
        }
        
        my %constant = %{$opengl_constant_map{$key}};

        print $handle $DUKTAPE_INDENT."$OPENGL_CONSTANT_SET_MACRO_NAME(ctx, $constant{'name'});\n";
    }
    if ($old_scope ne "")
    {
        print $handle "#endif /* $old_scope */\n\n";
    }
    
    print $handle "}\n\n";
}

#
# Program main
#

sub main()
{
    process_arguments();
    validate_arguments();
    
    process_input_files();
    validate_processed_data();
    
    if ($output_h_file ne "" || $use_stdout)
    {
        my $h = \*STDOUT;
        if ($output_h_file ne "")
        {
            open($h, '>', $output_h_file) or die "Could not open file '$output_h_file' for writing $!";
        }
        
        output_header_file($h);
        
        if ($output_h_file ne "")
        {
            close($h);
        }
    }
    
    if ($output_c_file ne "" || $use_stdout)
    {
        my $c = \*STDOUT;
        if ($output_c_file ne "")
        {
            open($c, '>', $output_c_file) or die "Could not open file '$output_c_file' for writing $!";
        }

        output_c_file_header($c);
    
        output_binding_function_macro($c);
        output_property_set_macro($c);
        
        output_array_handling_macros($c);
        
        output_wrapper_function_macros($c);
        
        output_wrapper_functions($c);
        
        output_function_bindings($c);
        output_constants($c);
        output_initialization_function($c);

        if ($output_c_file ne "")
        {
            close($c);
        }
    }

    exit();
}

main();
