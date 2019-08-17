#ifndef GUI_UTIL_HH
#define GUI_UTIL_HH

#include <QSettings>
#include "ErrorXOptions.hh"

namespace gui_util {

// Builds an ErrorXOptions object from QSettings
// does not address filename, filetype, species, receptor type
errorx::ErrorXOptionsSP optionsFromSettings( QSettings* const settings);

} // namespace gui_util

#endif // GUI_UTIL_HH