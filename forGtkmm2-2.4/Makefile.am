# program we want built
bin_PROGRAMS = peless

# sources required for peless
peless_SOURCES = peless.cc gmore.cc search.cc gmore.hh inittime.hh internat.hh search.hh


#extra files
EXTRA_DIST = philo.txt INTERNATIONALIZATION peless.spec.FedoraCore2 peless.spec.SuSE.90 peless.spec.SuSE.90 peless.png

# to compile, use flags needed by both gtkmm and boost
peless_CXXFLAGS=$(DEPS_CFLAGS) $(BOOST_CXXFLAGS)
# in above, DEPS_CFLAGS and BOOST_CXXFLAGS came from configure.ac

# to link, use flags needed by both gtkmm and boost
peless_LDADD=$(DEPS_LIBS) $(BOOST_LIBS)
# in above, DEPS_LIBS and BOOST_LIBS came from configure.ac