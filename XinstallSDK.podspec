Pod::Spec.new do |s|
  s.name = "XinstallSDK"
  s.version = "0.0.1"
  s.summary = "XinstallSDK"
  s.license = {"type"=>"MIT", "file"=>"LICENSE"}
  s.authors = {"huawenjie"=>"huahanjie1989@qq.com"}
  s.homepage = "https://www.xinstall.com"
  s.description = "XinstallSDK create by xinstall.com"
  s.source = { :path => '.' }

  s.ios.deployment_target    = '9.2'
  s.ios.vendored_framework   = 'frameworks/XinstallSDK.framework'
end
