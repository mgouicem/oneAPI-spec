#include "onedal/table.hpp"

namespace oneapi::dal {

/// @tparam Data The type of data values in blocks returned by the accessor.
///           Shall be const-qualified for read-only access.
///           An accessor shall support at least :expr:`float`, :expr:`double`, and :expr:`std::int32_t` types of $Data$.
template <typename Data>
class row_accessor {
public:
    using data_t = std::remove_const_t<Data>;

public:
    /// Creates a new read-only accessor object from the table.
    /// The check that the accessor supports the table kind of $obj$ shall be performed.
    /// The reference to the $obj$ table shall be stored within the accessor to
    /// obtain data from the table.
    row_accessor(const table& obj);

    /// Provides access to the rows of the table.
    /// The method shall return an array that directly points to the memory within the table
    /// if it is possible. In that case, the array shall refer to the memory as to immutable data.
    /// Otherwise, the new memory block shall be allocated, the data from the table rows shall be converted
    /// and copied into this block. The array shall refer to the block as to mutable data.
    ///
    /// @param[in] queue The SYCL* queue object.
    /// @param[in] rows  The range of rows that data shall be returned from the accessor.
    /// @param[in] alloc The requested kind of USM in the returned block.
    ///
    /// @pre ``rows`` are within the range of ``[0, obj.row_count)``.
    array<data_t> pull(sycl::queue& queue,
                       const range& rows             = { 0, -1 },
                       const sycl::usm::alloc& alloc = sycl::usm::alloc::shared) const;

    /// Provides access to the rows of the table.
    /// The method shall return the :expr:`block.data` pointer.
    ///
    /// @param[in] queue     The SYCL* queue object.
    /// @param[in,out] block The block which memory is reused (if it is possible) to obtain the data from the table.
    ///                      The block memory shall be reset either when
    ///                      its size is not big enough, or when it contains immutable data, or when direct
    ///                      memory from the table can be used.
    ///                      If the block is reset to use a direct memory pointer from the object,
    ///                      it shall refer to this pointer as to immutable memory block.
    /// @param[in] rows      The range of rows that data shall be returned from the accessor.
    /// @param[in] alloc     The requested kind of USM in the returned block.
    ///
    /// @pre ``rows`` are within the range of ``[0, obj.row_count)``.
    Data* pull(sycl::queue& queue,
            array<data_t>& block,
            const range& rows             = { 0, -1 },
            const sycl::usm::alloc& alloc = sycl::usm::alloc::shared) const;
};

} // namespace oneapi::dal
