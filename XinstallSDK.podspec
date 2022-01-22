Pod::Spec.new do |s|
  s.name            = "XinstallSDK"
  s.version         = "1.5.7.2"
  s.platform        = :ios, '9.0'
  s.summary         = "XinstallSDK is officially supported iOS SDK"
  s.description     = <<-DESC
      Xinstall's officially supported iOS SDK for pod.
  DESC

  s.license         = 'MIT'
  s.authors         = {"hi"=>"hi@xinstall.com"}
  s.homepage        = "https://www.xinstall.com"
  s.description     = "XinstallSDK create by xinstall.com"
  s.source          = { :git => "https://github.com/xinstall/iOSSDK.git", :tag => s.version.to_s }

  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64'}

  s.source_files = 'Classes/**/*.{h,m}'
  s.vendored_libraries = 'frameworks/libXinstallSDK.a'

end
