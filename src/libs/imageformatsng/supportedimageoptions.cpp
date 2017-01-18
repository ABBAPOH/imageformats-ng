#include "supportedimageoptions.h"

/*!
    \class SupportedImageOptions

    SupportedImageOptions is the set of the ImageOptions::Option.

    It provides only small subset of the operations available for the set -
    contains(), insert() and remove() operations.
*/

/*!
    \fn inline bool SupportedImageOptions::contains(ImageOptions::Option option) const

    Returns true if the given \a option is in the set.
*/

/*!
    \fn inline void SupportedImageOptions::insert(ImageOptions::Option option) const

    Inserts the given \a option into the set.
*/

/*!
    \fn inline void SupportedImageOptions::remove(ImageOptions::Option option) const

    Removes the given \a option into the set.
*/

QSet<ImageOptions::Option> SupportedImageOptions::toSet() const
{
    QSet<ImageOptions::Option> result;
    for (int i = 0; i < ImageOptions::OptionsCount; ++i) {
        const auto option = ImageOptions::Option(i);
        if (contains(option))
            result.insert(option);
    }
    return result;
}
