#!/usr/bin/ruby
# -*- coding: utf-8 -*-
# cgi-printarg-ruby.cgi -- CGI プログラムに対する引数 arg1 と arg2 を表示するプログラム
# ~yas/syspro/www/cgi-printarg-ruby.cgi
# Created on 2014/08/05 18:31:21

require "cgi"

def main()
        $SAFE = 1
        @cgi = CGI.new()
        print_header()
        print_content()
        exit( 0 )
end

def print_header()
        printf("Content-Type: text/html\n")
        printf("\n")
end

def print_content()
        printf("<HTML><HEAD></HEAD><BODY><PRE>\n")
        printf("arg1: [%s]\n", e(@cgi["arg1"]))
        printf("arg2: [%s]\n", e(@cgi["arg2"]))
        printf("</PRE></BODY></HTML>\n")
end

def e( str )
        return( str == nil ? "(null)" : CGI::escapeHTML(str) )
end

main()
