#
# PoHessian
# Portable C++ Hessian Implementation
#
# Copyright (C) 2012  Pierre-David Belanger
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Contributor(s): Pierre-David Belanger <pierredavidbelanger@gmail.com>
#

-include Makefile.config
-include Makefile.config.$(OS)
-include Makefile.config.$(OS).$(ARCH)

all: debug_shared release_shared debug_static release_static

debug_shared::

release_shared::

debug_static::

release_static::

$(DIR_BUILD_FULL)/$(DIR_DEBUG)/%$(OBJECT_EXT):: $(DIR_SOURCE)/%$(SOURCE_EXT)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_DEBUG)
	$(CC) $(CC_ARCH) $(CC_WARN) $(CC_PIC) $(CC_FLAGS_DEBUG) $(CC_FLAGS_OTHER) $(CC_INCLUDE_PATHS) $(CC_COMPILE) $< $(CC_OUT) $@

$(DIR_BUILD_FULL)/$(DIR_RELEASE)/%$(OBJECT_EXT):: $(DIR_SOURCE)/%$(SOURCE_EXT)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_RELEASE)
	$(CC) $(CC_ARCH) $(CC_WARN) $(CC_PIC) $(CC_FLAGS_RELEASE) $(CC_FLAGS_OTHER) $(CC_INCLUDE_PATHS) $(CC_COMPILE) $< $(CC_OUT) $@

ifeq ($(TARGET_SHARED),true)

debug_shared:: $(SHARED_DEBUG)

$(SHARED_DEBUG): $(OBJECTS_DEBUG)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_DEBUG)/$(DIR_SHARED)
	$(CC) $(CC_ARCH) $(CC_FLAGS_DEBUG) $(CC_FLAGS_SHARED) $(CC_OUT) $@ $^ $(CC_LIBRARY_PATHS_DEBUG) $(CC_LIBRARY_NAMES_DEBUG)

release_shared:: $(SHARED_RELEASE)

$(SHARED_RELEASE): $(OBJECTS_RELEASE)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_RELEASE)/$(DIR_SHARED)
	$(CC) $(CC_ARCH) $(CC_FLAGS_SHARED) $(CC_OUT) $@ $^ $(CC_LIBRARY_PATHS_RELEASE) $(CC_LIBRARY_NAMES_RELEASE)

endif

ifeq ($(TARGET_STATIC),true)

debug_static:: $(STATIC_DEBUG)

$(STATIC_DEBUG): $(OBJECTS_DEBUG)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_DEBUG)/$(DIR_STATIC)
	$(AR) $@ $^

release_static:: $(STATIC_RELEASE)

$(STATIC_RELEASE): $(OBJECTS_RELEASE)
	$(MKDIR) $(DIR_BUILD_FULL)/$(DIR_RELEASE)/$(DIR_STATIC)
	$(AR) $@ $^

endif

clean:
	$(RM) $(DIR_BUILD)
