::[Bat To Exe Converter]
::
::YAwzoRdxOk+EWAjk
::fBw5plQjdCyDJGyX8VAjFD1GSQC+GGStCLkT6ezo07zW+xhLDbNxcYzUug==
::YAwzuBVtJxjWCl3EqQJgSA==
::ZR4luwNxJguZRRnk
::Yhs/ulQjdF+5
::cxAkpRVqdFKZSjk=
::cBs/ulQjdF+5
::ZR41oxFsdFKZSDk=
::eBoioBt6dFKZSDk=
::cRo6pxp7LAbNWATEpCI=
::egkzugNsPRvcWATEpCI=
::dAsiuh18IRvcCxnZtBJQ
::cRYluBh/LU+EWAnk
::YxY4rhs+aU+IeA==
::cxY6rQJ7JhzQF1fEqQJhZksaHErScjva
::ZQ05rAF9IBncCkqN+0xwdVsFAlTMbCXqSOV8
::ZQ05rAF9IAHYFVzEqQIZGFt1SACQOXipRpkV+/yb
::eg0/rx1wNQPfEVWB+kM9LVsJDA2SfGq+AqcZ+/ub
::fBEirQZwNQPfEVWB+kM9LVsJDA2SHW++FLAP+4g=
::cRolqwZ3JBvQF1fEqQIcIQhADAWOMCujCaAOqOvu/fiCrFldXfJ/eYvfyLGdM6AQ5UvifJE4xHtOkcQCH3s=
::dhA7uBVwLU+EWHKT9Vc3aC9RTwyMM2e1AbwZ+6jX5ueOtkgZNA==
::YQ03rBFzNR3SWATElA==
::dhAmsQZ3MwfNWATElA==
::ZQ0/vhVqMQ3MEVWAtB9wSA==
::Zg8zqx1/OA3MEVWAtB9wSA==
::dhA7pRFwIByZRRmt5AIxLB9GSReRfGezFaEZ+og=
::Zh4grVQjdCyDJGyX8VAjFD1GSQC+EX6pD7Ygyunv/Naqmw00ZKw6YIq7
::YB416Ek+ZG8=
::
::
::978f952a14a936cc963da21a135fa983
@echo off
setlocal EnableDelayedExpansion
set ip_address_string="IPv4 Address"
for /f "tokens=2 delims=:" %%f in ('ipconfig ^| findstr /c:%ip_address_string%') do (
    set ip_address=%%f
    echo Your IP Address is: !ip_address:~1!
)
pause
