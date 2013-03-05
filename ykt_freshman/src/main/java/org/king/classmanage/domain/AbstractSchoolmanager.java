package org.king.classmanage.domain;

import org.apache.commons.lang.builder.EqualsBuilder;
import org.apache.commons.lang.builder.HashCodeBuilder;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;



/**
 * AbstractSchoolmanager generated by MyEclipse - Hibernate Tools
 */

public abstract class AbstractSchoolmanager extends org.king.framework.domain.BaseObject implements java.io.Serializable {


    // Fields    

     private SchoolmanagerId id;


    // Constructors

    /** default constructor */
    public AbstractSchoolmanager() {
    }

    

   
    // Property accessors

    public SchoolmanagerId getId() {
        return this.id;
    }
    
    public void setId(SchoolmanagerId id) {
        this.id = id;
    }




	/**
	 * @see java.lang.Object#equals(Object)
	 */
	public boolean equals(Object object) {
		if (!(object instanceof AbstractSchoolmanager)) {
			return false;
		}
		AbstractSchoolmanager rhs = (AbstractSchoolmanager) object;
		return new EqualsBuilder().append(this.id, rhs.id).isEquals();
	}




	/**
	 * @see java.lang.Object#hashCode()
	 */
	public int hashCode() {
		return new HashCodeBuilder(365914665, -1575293829).append(this.id)
				.toHashCode();
	}




	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
				.append("id", this.id).toString();
	}













}