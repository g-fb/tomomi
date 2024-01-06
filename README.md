#### Donate: [GitHub Sponsors](https://github.com/sponsors/g-fb) | [Liberapay](https://liberapay.com/gfb/) | [PayPal](https://paypal.me/georgefloreabanus)
![screenshot](https://i.imgur.com/QGm22ig.jpg)

# Build

Building is complicated right now as it uses KDE Frameworks 6 which are not released yet.

Easiest way to build is to setup [kdesrc-build](https://community.kde.org/Get_Involved/development/Build_software_with_kdesrc-build) and add to kdesrc-buildrc
```
module tomomi
    repository https://github.com/g-fb/tomomi
    cmake-options -DBUILD_WITH_QT6=ON
end module
```
then run `kdesrc-build tomomi` and `kdesrc-run tomomi`
