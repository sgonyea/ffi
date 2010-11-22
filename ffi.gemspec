# -*- encoding: utf-8 -*-

require 'date'

Gem::Specification.new do |s|
  s.name              = %q{ffi}
  s.version           = "0.6.0"

  s.authors           = ["Wayne Meissner"]
  s.date              = Date.today.to_s
  s.summary           = %q{Ruby-FFI is a ruby extension for programmatically loading dynamic libraries, binding functions within them, and calling those functions from Ruby code}
  s.description       = %q{Ruby-FFI is a ruby extension for programmatically loading dynamic libraries, binding functions within them, and calling those functions from Ruby code. Moreover, a Ruby-FFI extension works without changes on Ruby and JRuby. Discover why should you write your next extension using Ruby-FFI here[http://wiki.github.com/ffi/ffi/why-use-ffi].}
  s.email             = %q{wmeissner@gmail.com}
  s.homepage          = %q{http://wiki.github.com/ffi/ffi}

  s.extensions        = ["ext/ffi_c/extconf.rb", "gen/Rakefile"]

  s.files             = `git ls-files`.split("\n")
  s.test_files        = `git ls-files -- {spec}/*`.split("\n")
  s.require_paths     = ["lib", "ext"]

  s.rdoc_options      = ["-x", "ext", "--main", "README.rdoc"]
  s.rubyforge_project = %q{ffi}
  s.rubygems_version  = %q{1.3.5}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<rake>, [">= 0.8.7"])
      s.add_development_dependency(%q<bones>, [">= 2.5.1"])
    else
      s.add_dependency(%q<rake>, [">= 0.8.7"])
      s.add_dependency(%q<bones>, [">= 2.5.1"])
    end
  else
    s.add_dependency(%q<rake>, [">= 0.8.7"])
    s.add_dependency(%q<bones>, [">= 2.5.1"])
  end
end
