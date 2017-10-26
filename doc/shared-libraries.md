Shared Libraries
================

## iranbitconsensus

The purpose of this library is to make the verification functionality that is critical to Iranbit's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `iranbitconsensus.h` located in  `src/script/iranbitconsensus.h`.

#### Version

`iranbitconsensus_version` returns an `unsigned int` with the the API version *(currently at an experimental `0`)*.

#### Script Validation

`iranbitconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `iranbitconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `iranbitconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `iranbitconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/iranbit/bips/blob/master/bip-0016.mediawiki)) subscripts
- `iranbitconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/iranbit/bips/blob/master/bip-0066.mediawiki)) compliance

##### Errors
- `iranbitconsensus_ERR_OK` - No errors with input parameters *(see the return value of `iranbitconsensus_verify_script` for the verification status)*
- `iranbitconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `iranbitconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `iranbitconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`

### Example Implementations
- [NIranbit](https://github.com/NicolasDorier/NIranbit/blob/master/NIranbit/Script.cs#L814) (.NET Bindings)
- [node-libiranbitconsensus](https://github.com/bitpay/node-libiranbitconsensus) (Node.js Bindings)
- [java-libiranbitconsensus](https://github.com/dexX7/java-libiranbitconsensus) (Java Bindings)
- [iranbitconsensus-php](https://github.com/Bit-Wasp/iranbitconsensus-php) (PHP Bindings)
