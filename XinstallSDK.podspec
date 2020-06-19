Pod::Spec.new do |s|
  s.name            = "XinstallSDK"
  s.version         = "0.0.1"
  s.platform        = :ios, '9.2'
  s.summary         = "XinstallSDK is officially supported iOS SDK"
  s.description     = <<-DESC
      Xinstall's officially supported iOS SDK for pod.
  DESC
  
  s.license         = 'MIT'
  s.authors         = {"hi"=>"hi@xinstall.com"}
  s.homepage        = "https://www.xinstall.com"
  s.description     = "XinstallSDK create by xinstall.com"
  s.source          = { :git => "https://github.com/xinstall/iOSSDK.git", :tag => s.version.to_s }

  s.vendored_libraries = 'frameworks/libXinstallSDK.a'
  s.source_files = 'Classes/**/*.{h,m}'
end
