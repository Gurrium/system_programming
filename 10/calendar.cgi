#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require "cgi"

def response
  <<~"BODY"
    Content-Type: text/html

    <!DOCTYPE html>
    <html lang="en">
      <head>
        <meta charset="UTF-8">
        <title>Calendar</title>
      </head>
      <body>
        <pre>
          #{html_body}
        </pre>
      </body>
    </html>
  BODY
end

def html_body
  if is_valid?(params)
    `cal -hm #{params[:month]} #{params[:year]}`.chomp
  else
    'Argument is invalid'
  end
end

def params
  {
    year: @cgi['year'].to_i,
    month: @cgi['month'].to_i
  }
end

def is_valid?(params)
  params[:year] != 0 && params[:month] != 0 && (1..9999).include?(params[:year]) && (1..12).include?(params[:month])
end

$SAFE = 1
@cgi = CGI.new
print response
exit(0)
