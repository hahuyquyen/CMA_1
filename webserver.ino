void setupWiFiConf(void) {
  server.on(html_setup_wifi, []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = FPSTR(p_html);
    content += F("Connected Wifi: ");
    content1 = FPSTR(p_html);
    content1 += WiFiConf.sta_ssid;
    content1 += F("</br>IP Address: ");
    content1 += FPSTR(_p_html);
    content1 += FPSTR(p_html);
    content1 += FPSTR(_p_html);
    content1 += FPSTR(get_html);
    content1 += F("'set_wifi_conf'>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'ssid' class=\"req\">SSID : </label>");
    content1 += F("<input name='ssid'class=\"txt\" id='ssid' maxlength=32 value=") ;
    content1 += WiFiConf.sta_ssid ;
    content1 += F("></div>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'pwd' class=\"req\">Password :  </label>");
    content1 += F("<input type='password' class=\"txt\" name='pwd' id='pwd' value=");
    content1 += String(WiFiConf.sta_pwd);
    content1 += F("></div>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'ip' class=\"req\">Ip : </label>");
    content1 += F("<input name='ip' class=\"txt\" id='ip' value=");
    content1 += String(WiFiConf.sta_ip) ;
    content1 += F("></div>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'gateway' class=\"req\" >Gateway :  </label>");
    content1 += F("<input  name='gateway' class=\"txt\" id='gateway' value=") ;
    content1 += WiFiConf.sta_gateway ;
    content1 += F("></div>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'subnet' class=\"req\">Subnet :</label>");
    content1 += F("<input  name='subnet' class=\"txt\" id='subnet' value=");
    content1 +=  WiFiConf.sta_subnet;
    content1 += F("></div>");
    content1 += F("<div class=\"row\">");
    content1 += FPSTR(label1_html);
    content1 += F("'apssid' class=\"req\">AP SSID :</label>");
    content1 += F("<input  name='apssid' class=\"txt\" id='apssid' value=");
    content1 +=  WiFiConf.ap_ssid;
    content1 += F("></div>");
    content += FPSTR(wifisetting_html);
    content += FPSTR(title_html);
    content += F("<h1>Wifi Setup</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += FPSTR(_p_html);
   // content += network_html;
    content += FPSTR(end_html);
    server.send(200, F("text/html"), content);
  });
  server.on("/main", []() {
    String content = "Page 1";
    server.send(200, F("text/html"), content);
  });
  server.on(html_setup_setwifi, HTTP_GET, []() {
    String ssid = server.arg(F("ssid"));
    String password = server.arg(F("pwd"));
    String ip = server.arg(F("ip"));
    String gateway = server.arg(F("gateway"));
    String subnet = server.arg(F("subnet"));
    String AP = server.arg(F("apssid"));
    ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
    password.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
    ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
    gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
    subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
    AP.toCharArray(WiFiConf.ap_ssid, sizeof(WiFiConf.ap_ssid));
    saveWiFiConf();
    server.send(200, F("text/html"),"OK");
  });
   server.on("/set1", []() {
    String new_IPHC = server.arg(F("ip"));
    String new_pwdhc = server.arg(F("pwd"));
    Serial.println(new_IPHC);
    Serial.println(new_pwdhc);
    server.send(200, F("text/html"), new_IPHC);
  });
}
void setupWeb(void) {
  server.on("/", []() {
    String content = "Hello Server" ;
    server.send(200, F("text/html"), content);

  });
}
