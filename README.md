# moneycpp
A C++ 17 header-only, cross-platform library for handling monetary values, currencies, rounding and other related features.

**THIS PROJECT IS UNDER CONSTRUCTION!**

## Core requirements
The library is intended for being used in a variety of types of application including ERP systems, banking, finance, insurrance, games, and others.

The following is a list of its core requirements:
* Provide an API for handing and calculating with monetary amounts.
* Support different numeric capabilities.
* Provide a default set of rounding algorithms and policies and support additional user-defined ones.
* Support the entire ISO 4217 list of currencies.
* Support the entire ISO 3166-1 list of countries.
* It should be possible for users to add new (virtual) currencies and countries.
* Format and parse monetary values.

## Library API
The following are examples for using the library:

```cpp
// create and operate with money values
auto m = make_money(20.0, currency::USD);
m += make_money(10.5, currency::USD);
m *= 2.5;

// round money values
m = rounding_policy_standard(round_ceiling())(m);

// convert between currencies 
auto ex = exchange_money(
   m, 
   currency::EUR, 0.86,
   rounding_policy_standard(round_ceiling()));  
```

Floating point types are not appropriate for monetary values because they cannot exactly represent real numbers. Over time, or large number of transactions, the small differences can add up to important values. Because of this, the library supports 3rd party libraries that provide better representations, such as boost::multiprecision.
```cpp
using decimal = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<50>>;

inline decimal operator""_dec(char const * str, std::size_t)
{ return decimal(str); }

auto m = make_money("20.99"_dec, currency::USD);

auto ex = exchange_money(
   m, 
   currency::EUR, "0.8649"_dec,
   rounding_policy_to_currency_digits(round_half_even()));
```

## Rounding
Several rounding algorithms are provided with the library. These algorithms take a numerical value and return another numerical value. In addition to these, any user-defined rounding algorithm can be used with the library. The rounding algorithms are as follows:

| Name | Description | Functor |
| --- | --- | --- |
| None | no rounding | `round_none` |
| Up | rounds away from zero | `round_up` |
| Down | rounds towards zero | `round_down` |
| Ceiling | rounds towards positive infinity | `round_ceiling` |
| Floor | rounds towards negative infinity | `round_floor` |
| Half up | rounds towards "nearest neighbour" unless both neighbours are equidistant, in which case round up | `round_half_up` |
| Half down | rounds towards "nearest neighbour" unless both neighbours are equidistant, in which case round down | `round_half_down` |
| Half even | rounds towards the "nearest neighbour" unless both neighbours are equidistant, in which case, round towards the even neighbour | `round_half_even` |
| Half odd | rounds towards the "nearest neighbour" unless both neighbours are equidistant, in which case, round towards the odd neighbour | `round_half_odd` |

The following is a table with numerical examples for each rounding algorithm:

|  | -5.5 | -2.5 | -1.6 | -1.1 | -1.0 | 1.0 | 1.1 | 1.6 | 2.5 | 5.5 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Up | -6.0 | -3.0 | -2.0 | -2.0 | -1.0 | 1.0 | 2.0 | 2.0 | 3.0 | 6.0 |
| Down | -5.0 | -2.0 | -1.0 | -1.0 | -1.0 | 1.0 | 1.0 | 1.0 | 2.0 | 5.0 |
| Ceiling | -5.0 | -2.0 | -1.0 | -1.0 | -1.0 | 1.0 | 2.0 |  2.0 | 3.0 | 6.0 |
| Floor | -6.0 | -3.0 | -2.0 | -2.0 | -1.0 | 1.0 | 1.0 | 1.0 | 2.0 | 5.0 |
| Half up | -6.0 | -3.0 | -2.0 | -1.0 | -1.0 | 1.0 | 1.0 | 2.0 | 3.0 | 6.0 |
| Half down | -5.0 | -2.0 | -2.0 | -1.0 | -1.0 | 1.0 | 1.0 | 2.0 | 2.0 | 5.0 |
| Half even | -6.0 | -2.0 | -2.0 | -1.0 | -1.0 | 1.0 | 1.0 | 2.0 | 2.0 | 6.0 |
| Half odd | -5.0 | -3.0 | -2.0 | -1.0 | -1.0 | 1.0 | 1.0 | 2.0 | 3.0 | 5.0 |

More about these rounding algorithms can be found in the article [Rounding Algorithms 101 Redux](https://www.eetimes.com/document.asp?doc_id=1274515).

Apart from the rounding algorithms, the library provides several rounding policies that define how a `money` value should be rounded. The available policies are:

| Type name | Description |
| --- | --- |
| `rounding_policy_none`| No rounding is performed |
| `rounding_policy_standard` | Rounding to 4 decimal digits |
| `rounding_policy_to_currency_digits` | Rounding to the number of digits (i.e. minor unit) as defined for the currency |

Any additional user-defined policy can be used instead of the ones supplied with the library.
