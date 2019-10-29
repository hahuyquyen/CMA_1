const char space_html[] PROGMEM ="&nbsp;";
const char _legend_html[] PROGMEM ="</a></legend>";
const char legend_html[] PROGMEM ="<legend><a href=";
const char wifisetting_html[] PROGMEM ="mHome - Wifi Setup";
const char get_html[] PROGMEM ="<form method='get' action=";
const char notwifi_html[] PROGMEM = "Wifi : Not Connected.";
 const char support_html[] PROGMEM =   "Cty : TNHH Kim Sơn Tiến Địa chỉ : 65/39 đường 339 Phường Tăng Nhơn Phú B,Q9,TP.HCM </br>Phát Triển : Phạm An Nhàn";
const char label_html[] PROGMEM ="<li><label for=";
const char label1_html[] PROGMEM ="<label for=";
const char br_html[] PROGMEM ="><br /><br />";
const char _p_html[] PROGMEM ="</p>";
const char p_html[] PROGMEM ="<p>";
const char title_html[] PROGMEM ="</title></head><body>";
const char end_html[] PROGMEM = "</body></html>";
const char _fieldset[] PROGMEM ="</fieldset>";
const char fieldset[] PROGMEM ="<fieldset>";
const char information[] PROGMEM = "<li>Copyright © by Kim Son Tien Co.Ltd - mHome <li>www.nhathongminhmhome.com <li>Designed by mHome - R&D Department <li>Made in VietNam";
const char header[] PROGMEM = "<!DOCTYPE HTML>\r\n<html><head>"
                              "<style>"
                              "input {"
  "padding: 5px 0;"
  "font-size: 1.5em;"
  "font-family: inherit;"
  "width: 150px;"
"}"


                              
".weekDays-selector input {"
  "display: none!important;"
"}"

".weekDays-selector input[type=checkbox] + label {"
  "display: inline-block;"
  "border-radius: 6px;"
  "background: #dddddd;"
  "height: 40px;"
  "width: 30px;"
  "margin-right: 3px;"
  "line-height: 40px;"
  "text-align: center;"
  //cursor: pointer;
"}"

".weekDays-selector input[type=checkbox]:checked + label {"
  "background: #2AD705;"
  "color: #ffffff;"
"}"


                              
"nav {"
    "float: left;"
    "margin-right:30px;"
"}"
                              
                              ".dropbtn {"
                              "height: 50px;width: 150px;"
    "background-color: #4CAF50;"
    "color: white;"
    "padding: 10px;"
    "font-size: 20px;"  
"}"

                              "fieldset {"
"margin-top:10px;"
"background:#ffffcc;"
"}"
"legend {"
"background:#ffffac;"
 "border:1px solid #000; "
" padding:2px 5px;"
"}"
"body {background-color:lightgrey;}"
"h1 {"
"border-bottom: 1px solid #BBBBBB;"
    "font-size: 2em;"
    "text-align: center;"
    "text-shadow: 0 2px #FFFFFF, 1px 3px #666666;"
"}"

"form .row {"
  "padding: 7px 8px;"
  "margin-bottom: 7px;"
"}"
"form .row:hover {"
  "background: #f1f7fa;"
"}"

"form label {"
  "display: inline-block;"
  "font-weight: bold;"
  "width: 200px;"
  "color: #464646;"
"}"

"form .req { color: #ca5354; }"

"form input:focus, form textarea:focus { outline: none; }"

"form .txt {"
  "display: inline-block;"
  "padding: 8px 9px;"
  "padding-right: 30px;"
  "width: 340px;"
  "transition: all 0.3s linear;"
"}"


"form .txt:focus, form .txtarea:focus {"
  "width: 400px;"
"}"

"form .txt:valid {"
  "background-color: #deecda;"
  "border: 1px solid #95bc7d;"
"}"

"#submitbtn {"
  "height: 50px;"
  "width: 150px;"
  "color: #0a528f;"
  "background-color: #4581e5;"
"}"
"#submitbtn:hover, #submitbtn:focus {"
  "box-shadow: 0 0 15px rgba(70, 100, 200, 0.9);"
"}"                  
                              "</style>"
                              "<link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAADLy8sAHYsbACCNGADK28UADIEHAHOubwAShQoA//7/AObz6ADn8+gA6fPoAOzz6ADGxsYAXa5eAL7gugAghxAAtbyyABFmCAD09vEAN5ctADuPMAA4ly0AxuG9AKvXqQA2lzYAJ4cZADuXNgAikCIAKIwoAHS1cwDs++YAAHgAAKrKpwAAewAAj8WQACCHEQDa6tgAtbyzAMTcwQA4jzEAOpEuAFCkSwAYZgkAa7JiAIK9ggBAmDoAQJY9AA6FDADQ5NAApMalAC2PKQCozagA9PXwAPT48AAvbScAF2YKAGywZgBtsGYA4+3iAMviywA/mDsARpgyAM/jzgBkkGEA0eXLAC6KJAA2dDMAotCgAEWbRAA5iiQAv9+6AKmr3AAQZQgA8vXxAPn48QA+mDMAsdayAJ7DngChxKEANnQ0ACiRKABlkGIAq7iqAAB3AAAAegAAjsSQAHm5eQDKycoAw9vBAGGxYgB/mqMAhJesAPr6+AC21qoA/v/+AJqamgBygMIASpxAAMjIyADv8PYAYLBgANrr1gCUx5EAZ61gAGitYADQy9EAKGpOACOPJAD7+/wAJI8kAPv9+QDo8OUA/v//AI24iQD///8AWaxVAC+MJABerFUAXaleAF6pXgB5tHUA2uvXAPX16wDPys8AxOLGANHL0gAnhB8Agr1+AJqh1AAOfAgAzOHMAIe+gQCLvoEA6O7pAHK0bQCrt6oAMI4iAC6QKAAtlCsAS5s/AF+rXAB4t3YAwdrBAMrIygDv+e8A3O3eAM/K0ADf4uoAdJdxAHWXcQAniBoAbqtuAP3+9QAMhgwAoc6fAHO0bgAvjiMAFYYMAI28jgBGn0MAyMfIAGOrXQB4uHoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAyPe4c/NhEqT5QlaaAAX3JyXiITc5oWDmYpS11ycgByXIw8ZXJycnJycnxFg3JiclZESXJycnJycnJyOkEXfSScCXIvG1BtbW1rnYVAPRB4m3JyGgRxMU1OnoFycimVLlhycllTjXJycpAccnJmQpYLcnJkHx8zcnIeAnJyRjcPenJyDVNUBnJjgGBHchZIFDAHIAEoHWxyk1tqWnJDNjmDcm+KdCxycm4FK5hydVE7MhJyl34hGAeff3JyeRVScndoB3JyjpkfTHJyNC1VknIKYWcScnKRA3JyCIuhcFdycgl2iYQ+SjUmhoiiXHIMcnJycoI4JyMZLngkcnJyDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=\"/><meta charset='UTF-8' http-equiv=\"refresh\" content=\"60\" />";





const char begin_title[] PROGMEM ="<title>";


const char status_html_ok[] PROGMEM ="OK";


const char html_setup_wifi[] PROGMEM ="/wifi_conf";
const char html_setup_hc2[] PROGMEM ="/hc2_conf";
const char html_setup_switch[] PROGMEM ="/learning";
const char html_setup_leaningir[] PROGMEM ="/IR";
const char html_setup_reboot[] PROGMEM ="/Reboot";
const char html_setup_reset[] PROGMEM ="/Reset1";
const char html_setup_SETreset1[] PROGMEM ="/set_Reset1";
const char html_setup_SETreset[] PROGMEM ="/set_Reset";
const char html_setup_GETinfor[] PROGMEM ="/get_infor";
const char html_setup_module[] PROGMEM ="/module_id";
const char html_setup_SETmodule[] PROGMEM ="/set_module_id";
const char html_setup_setwifi[] PROGMEM ="/set_wifi_conf";
const char html_setup_sendmisu[] PROGMEM ="/sendmisubi";
const char html_setup_senddaikin[] PROGMEM ="/senddaikin";
const char html_setup_getcode[] PROGMEM ="/getcode";
const char html_setup_getstatus[] PROGMEM ="/getstatus";
const char html_setup_codeGC[] PROGMEM ="/codeGC";
const char html_setup_codeIR[] PROGMEM ="/codeIR";
const char html_setup_SETHC2[] PROGMEM ="/set_hc2_conf";

/*                            //"body {background-color:lightgrey; font-size: 24px;} .voffset {margin-top: 30px;}"
                              //"h1 {color:blue}p {color:black}"
                          //   ".button {"
                             //  " display: inline-block;"
                            //   "border-radius: 4px;"
                             //   "background-color: #f4511e;"
                             //   "border: none;"
                             //   "color: #FFFFFF;"
                             //   "text-align: center;"
                             //   "font-size: 28px;"
                              //  "padding: 10px;"
                             //   "width: 200px;"
                             //   "transition: all 0.5s;"
                            //   "cursor: pointer;"
                          //     " margin: 5px;"
                           //   "}"
                              "fieldset {"
"position:relative;"
"margin-top:10px;"
"background:#ffffcc;"
"padding-top:20px;"
"}"
"legend {"
 "position:absolute;"
 "top:-10px;"
"background:#ffffac;"
" color:white;"
 "border:1px solid #000; "
" padding:2px 5px;"
"}"
//  "html, body, div, span, applet, object, iframe, h1, h2, h3, h4, h5, h6, p, blockquote, pre, a, abbr, acronym, address, big, cite, code, del, dfn, em, img, ins, kbd, q, s, samp, small, strike, strong, sub, sup, tt, var, b, u, i, center, dl, dt, dd, ol, ul, li, fieldset, form, legend, table, caption, tbody, tfoot, thead, tr, th, td, article, aside, canvas, details, embed, figure, figcaption, footer, header, hgroup, menu, nav, output, ruby, section, summary, time, mark, audio, video {"
//  "font-size: 1.1em;"
//  "color: #464646;"
 // "-webkit-font-smoothing: antialiased;"
 // "-webkit-text-size-adjust: 100%;"
 // "-ms-text-size-adjust: 100%;"
 // "-webkit-box-sizing: border-box;"
 // "-moz-box-sizing: border-box;"

//"}"
//"label {"
 // "margin: 0;"
//  "padding: 0;"
//  "border: 0;"
//  "font-size: 100%;"
 // "font: inherit;"
 // "vertical-align: baseline;"
 //" outline: none;"
 // "-webkit-font-smoothing: antialiased;"
 // "-webkit-text-size-adjust: 100%;"
//  "-ms-text-size-adjust: 100%;"
// "-webkit-box-sizing: border-box;"
 // "-moz-box-sizing: border-box;"
 // "box-sizing: border-box;"

"}"
//"html { height: 101%; }"
//"body {"
//  "font-family: 'Oxygen', Arial, Helvetica, sans-serif;"
//  "font-size: 100%;"
 // "line-height: 1;"
 // "padding-bottom: 85px;"
  //"background: #f5f7f7;"
//"}"

//"#w{ margin:0 auto; width:800px; background: #fff; padding:10px;}"

"h1 {"
    "border-bottom: 1px solid #BBBBBB;"
    "color: #5B5463;"
  //  "font-family: 'Skranji','Trebuchet MS',sans-serif;"
    "font-size: 2em;"
    //"font-weight: bold;"
    //"line-height: 1.7em;"
    //"margin-bottom: 7px;"
    "text-align: center;"
    "text-shadow: 0 2px #FFFFFF, 1px 3px #666666;"
    //"text-transform: uppercase;"
"}"

//"::selection { background: #d2bef5; }"
//"::-moz-selection { background: #d2bef5; }"
//"::-webkit-selection { background: #d2bef5; }"

//"br { display: block; line-height: 2.2em; }"

//"table { border-collapse: collapse; border-spacing: 0; }"
//"img { border: 0; max-width: 100%; }"

"form .row {"
  //"display: block;"
  "padding: 7px 8px;"
  "margin-bottom: 7px;"
"}"
"form .row:hover {"
  "background: #f1f7fa;"
"}"

"form label {"
  "display: inline-block;"
  //"font-size: 1.2em;"
  "font-weight: bold;"
  "width: 200px;"
 // "padding: 6px 0;"
  "color: #464646;"
 // "vertical-align: top;"
"}"
"form .req { color: #ca5354; }"

//"form .note {"
 // "font-size: 1.2em;"
 // "line-height: 1.33em;"
 // "font-weight: normal;"
 // "padding: 2px 7px;"
//  "margin-bottom: 10px;"
//"}"

"form input:focus, form textarea:focus { outline: none; }"


//"::-webkit-input-placeholder { color: #aaafbd; font-style: italic; }" 
//":-moz-placeholder { color: #aaafbd; font-style: italic; }"          
//"::-moz-placeholder { color: #aaafbd; font-style: italic; }"          
//":-ms-input-placeholder { color: #aaafbd; font-style: italic; }"     
"form .txt {"
  "display: inline-block;"
  "padding: 8px 9px;"
  "padding-right: 30px;"
  "width: 340px;"
  //"font-family: 'Oxygen', sans-serif;"
 // "font-size: 1.35em;"
  //"font-weight: normal;"
  //"color: #898989;"
  //"background-color: #f0f0f0;"
  //"background-image: url('checkmark.png');"
 // "background-position: 110% center;"
  //"background-repeat: no-repeat;"
 //// "border: 1px solid #ccc;"
  //"text-shadow: 0 1px 0 rgba(255,255,255,0.75);"
  //"-webkit-box-sizing: content-box;"
 // "-moz-box-sizing: content-box;"
  //"box-sizing: content-box;"
  //"-webkit-border-radius: 3px;"
  //"-moz-border-radius: 3px;"
  //"border-radius: 3px;"
 // "-webkit-box-shadow: 0 1px 2px rgba(25, 25, 25, 0.25) inset, -1px 1px #fff;"
 // "-moz-box-shadow: 0 1px 2px rgba(25, 25, 25, 0.25) inset, -1px 1px #fff;"
  //"box-shadow: 0 1px 2px rgba(25, 25, 25, 0.25) inset, -1px 1px #fff;"
  //"-webkit-transition: all 0.3s linear;"
 // "-moz-transition: all 0.3s linear;"
  "transition: all 0.3s linear;"
"}"


"form .txt:focus, form .txtarea:focus {"
  "width: 400px;"
  //"color: #545454;"
 // "background-color: #fff;"
 // "background-position: 110% center;"
 // "background-repeat: no-repeat;"
 / "border-color: #059;"
 // "-webkit-box-shadow: 0 1px 4px -1px #a8a8a8 inset, 0 1px rgba(255, 255, 255, 0.6), 0 0 11px rgba(70, 100, 200, 0.7);"
 // "-moz-box-shadow: 0 1px 4px -1px #a8a8a8 inset, 0 1px rgba(255, 255, 255, 0.6), 0 0 11px rgba(70, 100, 200, 0.7);"
//  "box-shadow: 0 1px 4px -1px #a8a8a8 inset, 0 1px rgba(255, 255, 255, 0.6), 0 0 11px rgba(70, 100, 200, 0.7);"
"}"


"form .txt:valid {"
  "background-color: #deecda;"
 // "background-position: 98% center;"
 // "background-repeat: no-repeat;"
//  "color: #7d996e;"
  "border: 1px solid #95bc7d;"
"}"




"#submitbtn {"
  "height: 70px;"
  "width: 275px;"
  "padding: 0;"
  "cursor: pointer;"
  "font-family: 'Oxygen', Arial, sans-serif;"
  "font-size: 2.0em;"
  "color: #0a528f;"
  "text-shadow: 1px 1px 0 rgba(255,255,255,0.65);"
  "border-width: 1px;"
  "border-style: solid;"
  "border-color: #317bd6 #3784e3 #2d74d5 #3774e3;"
  "-webkit-border-radius: 3px;"
  "-moz-border-radius: 3px;"
 " border-radius: 3px;"
  "background-color: #4581e5;"
  "background-image: -webkit-gradient(linear, left top, left bottom, from(#6faefd), to(#4581e5));"
 " background-image: -webkit-linear-gradient(top, #6faefd, #4581e5);"
  "background-image: -moz-linear-gradient(top, #6faefd, #4581e5);"
  "background-image: -ms-linear-gradient(top, #6faefd, #4581e5);"
  "background-image: -o-linear-gradient(top, #6faefd, #4581e5);"
  "background-image: linear-gradient(top, #6faefd, #4581e5);"
  "-moz-box-shadow: 1px 1px 3px rgba(0,0,0,0.4), 0 1px 0 rgba(255, 255, 255, 0.5) inset;"
  "-webkit-box-shadow: 1px 1px 3px rgba(0,0,0,0.4), 0 1px 0 rgba(255, 255, 255, 0.5) inset;"
  "box-shadow: 1px 1px 3px rgba(0,0,0,0.4), 0 1px 0 rgba(255, 255, 255, 0.5) inset;"
  "-webkit-transition: all 0.3s linear;"
  "-moz-transition: all 0.3s linear;"
  "transition: all 0.3s linear;"
"}"
"#submitbtn:hover, #submitbtn:focus {"
  "-webkit-box-shadow: 0 0 15px rgba(70, 100, 200, 0.9);"
  "-moz-box-shadow: 0 0 15px rgba(70, 100, 200, 0.9);"
  "box-shadow: 0 0 15px rgba(70, 100, 200, 0.9);"
"}"

"#submitbtn:active {"
  "-webkit-box-shadow: 0 0 15px rgba(70, 100, 200, 0.9), 0 1px 3px rgba(0,0,0,0.4) inset;"
  "-moz-box-shadow: 0 0 15px rgba(70, 100, 200, 0.9), 0 1px 3px rgba(0,0,0,0.4) inset;"
 " box-shadow: 0 0 15px rgba(70, 100, 200, 0.9), 0 1px 3px rgba(0,0,0,0.4) inset;"
"}"


".modalDialog {"
        "position: fixed;"
        "font-family: Arial, Helvetica, sans-serif;"
        "top: 0;"
        "right: 0;"
        "bottom: 0;"
        "left: 0;"
        "background: rgba(0,0,0,0.8);"
        "z-index: 99999;"
        "opacity:0;"
        "-webkit-transition: opacity 400ms ease-in;"
        "-moz-transition: opacity 400ms ease-in;"
        "transition: opacity 400ms ease-in;"
        "pointer-events: none;"
    "}"

".modalDialog:target {"
        "opacity:1;"
        "pointer-events: auto;"
    "}"

".modalDialog > div {"
        "width: 400px;"
        "position: relative;"
        "margin: 10% auto;"
        "padding: 5px 20px 13px 20px;"
        "border-radius: 10px;"
        "background: #fff;"
        "background: -moz-linear-gradient(#fff, #999);"
        "background: -webkit-linear-gradient(#fff, #999);"
        "background: -o-linear-gradient(#fff, #999);"
    "}"

".close {"
        "background: #606061;"
        "color: #FFFFFF;"
        "line-height: 25px;"
        "position: absolute;"
       " right: -12px;"
        "text-align: center;"
        "top: -10px;"
        "width: 24px;"
        "text-decoration: none;"
        "font-weight: bold;"
        "-webkit-border-radius: 12px;"
        "-moz-border-radius: 12px;"
        "border-radius: 12px;"
        "-moz-box-shadow: 1px 1px 3px #000;"
        "-webkit-box-shadow: 1px 1px 3px #000;"
        "box-shadow: 1px 1px 3px #000;"
    "}"

".close:hover { background: #00d9ff; }"
                              */
                              
